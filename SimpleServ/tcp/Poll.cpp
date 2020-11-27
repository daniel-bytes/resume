#include "Poll.h"
#include "TcpError.h"
#include "shared/Logger.h"
#include "shared/Configuration.h"

#include <cassert>
#include <string>

#define LOGGER "Poll"

using namespace std;
using namespace Logger::NdJson;

Poll::Poll(port_t httpPort, const SslConfiguration &sslConfig)
{
  Trace(LOGGER, "Poll::ctor");

  // Setup HTTP
  {
    auto httpSocket = new ListenSocket(httpPort);
    auto fd = httpSocket->FileDescriptor();

    Trace(LOGGER, "Poll::ctor - created HTTP socket fd " + std::to_string(fd));
    _listenSockets[fd] = httpSocket;
    _pfds.push_back({
      .fd = fd,
      .events = POLLIN
    });
  }

  // Setup HTTPS
  if (sslConfig.Enabled()) {
    auto httpsSocket = new SslListenSocket(sslConfig);
    auto fd = httpsSocket->FileDescriptor();

    Trace(LOGGER, "Poll::ctor - created HTTPS socket fd " + std::to_string(fd));
    _listenSockets[fd] = httpsSocket;
    _pfds.push_back({
      .fd = fd,
      .events = POLLIN
    });
  }
}

Poll::~Poll()
{
  for(auto &[key, listenSocket] : _listenSockets) {
    listenSocket->CloseSocket();
    delete listenSocket;
  }

  for(auto &[key, acceptSocket] : _acceptSockets) {
    acceptSocket->CloseSocket();
    delete acceptSocket;
  }
}

void 
Poll::BlockingPoll(TcpMessageListener &listener) 
{
  Trace(LOGGER, "Poll::BlockingPoll");

  while(true) {
    if (!PollSockets()) continue;

    ProcessSockets(listener);
    CleanupDisposedSockets();
  }

  Trace(LOGGER, "Poll::BlockingPoll end");
}

bool 
Poll::PollSockets() 
{
  Trace(LOGGER, "Poll::PollSockets");

  result_t result = poll(_pfds.data(), _pfds.size(), _socketTimeout);

  if (result < 0) {
    SocketError(LOGGER, "poll", _pfds[0].fd, result);
    return false;
  } 
  
  return (result != 0);
}

void 
Poll::ProcessSockets(TcpMessageListener &listener) 
{
  Trace(LOGGER, "Poll::ProcessSockets");

  for (auto pfd : _pfds) {
    Trace(LOGGER, "Poll::ProcessSockets pfd", {
      { "pfd.fd", pfd.fd },
      { "pfd.revents", pfd.revents }
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

    auto listenSocket = _listenSockets.find(pfd.fd);
    if (listenSocket != _listenSockets.end()) {
      OnListenSocketReceive(listenSocket->second);
    } else {
      auto acceptSocket = _acceptSockets.find(pfd.fd);
    
      if (acceptSocket != _acceptSockets.end()) {
        OnAcceptSocketReceive(acceptSocket->second, listener);
      }
    }
  }

  Trace(LOGGER, "Poll::ProcessSockets end");
}

void 
Poll::OnListenSocketReceive(ListenSocket *listenSocket) 
{
  Trace(LOGGER, "Poll::OnListenSocketReceive", {
    { "fd", listenSocket->FileDescriptor() }
  });

  // Release the unique_ptr as we manage ownership
  auto acceptSocket = listenSocket->Accept().release();
    
  if (acceptSocket->IsActive()) {
    Trace(LOGGER, "Poll::OnListenSocketReceive - socket accepted and active", {
      { "fd", listenSocket->FileDescriptor() }
    });
    
    _pfds.push_back({ 
      .fd = acceptSocket->FileDescriptor(),
      .events = POLLIN
    });

    _acceptSockets.insert_or_assign(acceptSocket->FileDescriptor(), acceptSocket);
  }
}

void 
Poll::OnAcceptSocketReceive(AcceptSocket *acceptSocket, TcpMessageListener &listener) 
{
  Trace(LOGGER, "Poll::OnAcceptSocketReceive", {
    { "fd", acceptSocket->FileDescriptor() }
  });
  
  string request;
  socket_t fd = acceptSocket->FileDescriptor();
  std::array<char, ACCEPT_BUFFER_SIZE> _buffer = {0};

  while(true) {
    Trace(LOGGER, "Poll::OnAcceptSocketReceive - fetching data", {
      { "fd", acceptSocket->FileDescriptor() }
    });

    auto result = acceptSocket->Recv(_buffer);

    Trace(LOGGER, "Poll::OnAcceptSocketReceive - data fetched", {
      { "fd", acceptSocket->FileDescriptor() },
      { "bytes_fetched", (int)result }
    });

    if (result > 0) {
      request.append(_buffer.begin(), _buffer.begin() + result);

      Trace(LOGGER, "Poll::OnAcceptSocketReceive - data appended", {
        { "fd", acceptSocket->FileDescriptor() }
      });
    } else {
      Trace(LOGGER, "Poll::OnAcceptSocketReceive - flag socket for dispose", {
        { "fd", acceptSocket->FileDescriptor() }
      });

      _socketsToDispose.insert(fd);
      break;
    }
  }

  Trace(LOGGER, "Poll::OnAcceptSocketReceive - request data read complete", {
    { "fd", acceptSocket->FileDescriptor() },
    { "request_size", (int)request.size() }
  });
  
  if (request.size()) {
    auto ipAddress = acceptSocket->GetRemoteAddress();
    string response = listener.TcpMessageReceived(request, ipAddress);
    
    acceptSocket->Send(response);
  }

  Trace(LOGGER, "Poll::OnAcceptSocketReceive end", { 
    { "fd", acceptSocket->FileDescriptor() } 
  });
}

void 
Poll::CleanupDisposedSockets() 
{
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

        auto acceptSocket = _acceptSockets[it->fd];
        acceptSocket->CloseSocket();
        delete acceptSocket;
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
    { "num_sockets_disposed_error", numSocketsToDispose != numSocketsDisposed ? Configuration::True() : Configuration::False() }
  });
}