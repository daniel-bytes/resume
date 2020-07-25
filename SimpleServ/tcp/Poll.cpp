#include "Poll.h"
#include "IpAddress.h"
#include "TcpError.h"
#include "Log.h"

#include <string>

using namespace std;

Poll::Poll(int serverPort)
  : _listenSocket(make_unique<ListenSocket>(serverPort))
{
  _pfds.push_back({ 
    .fd = _listenSocket->FileDescriptor(),
    .events = POLLIN
  });
}

void 
Poll::BlockingPoll(TcpMessageListener &listener) {
  while(true) {
    if (!PollSockets()) continue;

    ProcessSockets(listener);
    CleanupDisposedSockets();
  }
}

bool 
Poll::PollSockets() {
  int result = poll(&_pfds[0], _pfds.size(), _socketTimeout);

  if (result < 0) {
    Log::LogSocketError("poll", _pfds[0].fd, result);
    return false;
  } 
  
  return (result != 0);
}

void 
Poll::ProcessSockets(TcpMessageListener &listener) {
  for (auto pfd : _pfds) {
    if(pfd.revents == 0) {
      continue;
    }

    if (pfd.revents != POLLIN) {
      _socketsToDispose.insert(pfd.fd);
      continue;
    }

    if (pfd.fd == _listenSocket->FileDescriptor()) {
      OnListenSocketReceive();
    } else {
      OnAcceptSocketReceive(pfd.fd, listener);
    }
  }
}

void 
Poll::OnListenSocketReceive() {
  unique_ptr<AcceptSocket> socket = make_unique<AcceptSocket>(_listenSocket->FileDescriptor());
    
  if (socket->IsActive()) {
    _pfds.push_back({ 
      .fd = socket->FileDescriptor(),
      .events = POLLIN
    });

    _acceptSockets[socket->FileDescriptor()] = move(socket);
  }
}

void 
Poll::OnAcceptSocketReceive(int fd, TcpMessageListener &listener) {
  string request;

  while (true) {
    std::array<char, 80> _buffer = {0};
    
    int result = recv(fd, _buffer.begin(), _buffer.size(), 0);
    if (result < 0) {
      if (errno != EWOULDBLOCK) {
        Log::LogSocketError("recv", fd, result);
        _socketsToDispose.insert(fd);
      }
      break;
    }

    if (result == 0) {
      _socketsToDispose.insert(fd);
      break;
    }

    request.append(std::begin(_buffer), std::end(_buffer));
  }

  if (request.size()) {
    IpAddress ipAddress(*_acceptSockets[fd]);
    string response = listener.TcpMessageReceived(request, ipAddress);
    int result = _acceptSockets[fd]->Send(response);

    if (result < 0) {
      Log::LogSocketError("send", fd, result);
      _socketsToDispose.insert(fd);
    }
  }
}

void 
Poll::CleanupDisposedSockets() {
  auto it = _pfds.end();
  while (it > _pfds.begin()) {
    it--;
    if (_socketsToDispose.find(it->fd) != _socketsToDispose.end()) {
      _pfds.erase(it);
    }
  }
  _socketsToDispose.clear();
}