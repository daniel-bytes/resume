#include "Poll.h"
#include "TcpError.h"
#include "Logger.h"
#include "Configuration.h"

#include <cassert>
#include <string>

#define LOGGER "Poll"

using namespace std;
using namespace Logger::NdJson;

Poll::Poll(const port_t serverPort)
  : _listenSocket(ListenSocket(serverPort))
{
  Trace(LOGGER, "Poll::ctor");

  _pfds.push_back({ 
    .fd = _listenSocket.FileDescriptor(),
    .events = POLLIN
  });
}

Poll::~Poll()
{
  _listenSocket.CloseSocket();
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

  result_t result = poll(_pfds.data(), _pfds.size(), _socketTimeout);

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
    Trace(LOGGER, "Poll::ProcessSockets pfd", {
      { "pfd.fd", pfd.fd },
      { "pfd.revents", pfd.revents },
      { "listen_socket.fd", _listenSocket.FileDescriptor() }
    });

    if (pfd.revents == 0) {
      continue;
    }

    if (pfd.revents != POLLIN) {
      Error("A socket error was detected while polling", Socket(pfd.fd).GetSocketError(), {
        { "pfd.fd", pfd.fd },
        { "pfd.revents", pfd.revents }
      });
      
      _socketsToDispose.insert(pfd.fd);
      continue;
    }

    if (pfd.fd == _listenSocket.FileDescriptor()) {
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

  auto socket = _listenSocket.Accept();
    
  if (socket.IsActive()) {
    _pfds.push_back({ 
      .fd = socket.FileDescriptor(),
      .events = POLLIN
    });

    _acceptSockets.insert_or_assign(socket.FileDescriptor(), socket);
  }
}

void 
Poll::OnAcceptSocketReceive(socket_t fd, TcpMessageListener &listener) {
  Trace(LOGGER, "Poll::OnAcceptSocketReceive");
  
  string request;
  std::array<char, 80> _buffer = {0};

  while (true) {
    result_t result = recv(fd, _buffer.begin(), _buffer.size(), 0);
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

    auto ipAddress = _acceptSockets[fd].GetRemoteAddress();
    string response = listener.TcpMessageReceived(request, ipAddress);
    
    _acceptSockets[fd].Send(response);
  }

  Trace(LOGGER, "Poll::OnAcceptSocketReceive end");
}

void 
Poll::CleanupDisposedSockets() {
  int numPfds = (int)_pfds.size();
  int numSocketsToDispose = (int)_socketsToDispose.size();
  int numSocketsDisposed = 0;

  Trace(LOGGER, "Poll::CleanupDisposedSockets", { 
    { "num_pfds", numPfds },
    { "num_sockets_to_dispose", numSocketsToDispose }
  });

  if (_socketsToDispose.size()) {
    for (auto it = _pfds.begin(); _socketsToDispose.size() > 0 && it != _pfds.end();) {
      if (_socketsToDispose.find(it->fd) != _socketsToDispose.end()) {
        Trace(LOGGER, "Poll::CleanupDisposedSockets disposed fd", {{ "fd", it->fd }});

        _acceptSockets[it->fd].CloseSocket();
        _socketsToDispose.erase(it->fd);
        _acceptSockets.erase(it->fd);
        it = _pfds.erase(it);

        numSocketsDisposed++;
      } else {
        it++;
      }
    }
  }

  Trace(LOGGER, "Poll::CleanupDisposedSockets end", {
    { "num_pfds", numPfds },
    { "num_sockets_to_dispose", numSocketsToDispose },
    { "num_sockets_disposed", numSocketsDisposed },
    { "num_sockets_disposed_error", numSocketsToDispose != numSocketsDisposed ? Configuration::Global->True() : Configuration::Global->False() }
  });
}