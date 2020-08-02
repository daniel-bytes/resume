#include "Poll.h"
#include "TcpError.h"
#include "Logger.h"

#include <cassert>
#include <string>

#define LOGGER "Poll"

using namespace std;
using namespace Logger::NdJson;

Poll::Poll(int serverPort)
  : _listenSocket(make_unique<ListenSocket>(serverPort))
{
  Trace(LOGGER, "Poll::ctor");

  _pfds.push_back({ 
    .fd = _listenSocket->FileDescriptor(),
    .events = POLLIN
  });
}

void 
Poll::BlockingPoll(TcpMessageListener &listener) {
  Trace(LOGGER, "Poll::BlockingPoll");

  while(true) {
    if (!PollSockets()) continue;

    ProcessSockets(listener);
    CleanupDisposedSockets();
  }

  Trace(LOGGER, "Poll::BlockingPoll end");
}

bool 
Poll::PollSockets() {
  Trace(LOGGER, "Poll::PollSockets", { { "fd", _pfds[0].fd } });

  int result = poll(&_pfds[0], _pfds.size(), _socketTimeout);

  if (result < 0) {
    SocketError(LOGGER, "poll", _pfds[0].fd, result);
    return false;
  } 
  
  return (result != 0);
}

void 
Poll::ProcessSockets(TcpMessageListener &listener) {
  Trace(LOGGER, "Poll::ProcessSockets");

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

  Trace(LOGGER, "Poll::ProcessSockets end");
}

void 
Poll::OnListenSocketReceive() {
  Trace(LOGGER, "Poll::OnListenSocketReceive");

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
  Trace(LOGGER, "Poll::OnAcceptSocketReceive");
  
  string request;
  std::array<char, 80> _buffer = {0};

  while (true) {
    int result = recv(fd, _buffer.begin(), _buffer.size(), 0);
    if (result < 0) {
      if (errno != EWOULDBLOCK) {
        SocketError(LOGGER, "recv", fd, result);
        _socketsToDispose.insert(fd);
      }
      break;
    }

    if (result == 0) {
      _socketsToDispose.insert(fd);
      break;
    }

    request.append(_buffer.begin(), _buffer.begin() + result);
  }

  if (request.size()) {
    Trace(LOGGER, "Poll::OnAcceptSocketReceive data received");

    auto ipAddress = _acceptSockets[fd]->GetRemoteAddress();
    string response = listener.TcpMessageReceived(request, ipAddress);
    
    _acceptSockets[fd]->Send(response);
  }

  Trace(LOGGER, "Poll::OnAcceptSocketReceive end");
}

void 
Poll::CleanupDisposedSockets() {
  Trace(LOGGER, "Poll::CleanupDisposedSockets", { 
    { "num_pfds", (int)_pfds.size() },
    { "num_sockets_to_dispose", (int)_socketsToDispose.size() }
  });

  int numSocketsDisposed = 0;

  if (_socketsToDispose.size() > 0 && _pfds.size() > 1) {
    auto it = _pfds.end();
    while (it > _pfds.begin()) {
      it--;
      if (_socketsToDispose.find(it->fd) != _socketsToDispose.end()) {
        _pfds.erase(it);
        _acceptSockets.erase(it->fd);
        Trace(LOGGER, "Poll::CleanupDisposedSockets disposed fd", {{ "fd", it->fd }});

        numSocketsDisposed++;
       }
     }
     assert(numSocketsDisposed == _socketsToDispose.size());
     _socketsToDispose.clear();
  }

  Trace(LOGGER, "Poll::CleanupDisposedSockets end", {
    { "num_sockets_disposed", numSocketsDisposed }
  });
}