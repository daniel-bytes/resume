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
    listenSocket->CloseSocket().Handle(
      [](auto r) { return r; },
      [&](auto err) { 
        Error(LOGGER, err, {{ "fd", key }}); 
        return 0;
      }
    );
    delete listenSocket;
  }

  for(auto &[key, acceptSocket] : _acceptSockets) {
    acceptSocket->CloseSocket().Handle(
      [](auto r) { return r; },
      [&](auto err) { 
        Error(LOGGER, err, {{ "fd", key }}); 
        return 0;
      }
    );
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

  int timeout = _socketsToRetry.size() > 0 ? 100 : -1;

  result_t result = poll(_pfds.data(), _pfds.size(), timeout);

  if (result < 0) {
    SocketError(LOGGER, "poll", _pfds[0].fd, result);
    return false;
  } 
  
  return result > 0;
}

void 
Poll::ProcessSockets(TcpMessageListener &listener) 
{
  Trace(LOGGER, "Poll::ProcessSockets");

  for (auto pfd : _pfds) {
    auto retryIterator = _socketsToRetry.find(pfd.fd);
    auto isRetry = retryIterator != _socketsToRetry.end();

    Trace(LOGGER, "Poll::ProcessSockets pfd", {
      { "pfd.fd", pfd.fd },
      { "pfd.revents", pfd.revents },
      { "is_retry", isRetry }
    });

    if (isRetry) {
      _socketsToRetry.erase(retryIterator);
    } else {
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
      { "fd", listenSocket->FileDescriptor() },
      { "accept_fd", acceptSocket->FileDescriptor() }
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
  socket_t fd = acceptSocket->FileDescriptor();
  port_t port = acceptSocket->Port();

  Trace(LOGGER, "Poll::OnAcceptSocketReceive", {
    { "fd", fd },
    { "port", port }
  });
  
  string request;
  std::array<char, ACCEPT_BUFFER_SIZE> _buffer = {0};
  bool shouldRetry = false;

  while(true) {
    Trace(LOGGER, "Poll::OnAcceptSocketReceive - fetching data", {
      { "fd", fd },
      { "port", port }
    });

    auto result = acceptSocket->Recv(_buffer).Handle(
      [](auto r) { return r; },
      [&](auto err) { 
        Error(LOGGER, err, {{ "fd", fd }, { "port", port }}); 
        shouldRetry = err.ShouldRetry();
        return 0;
      }
    );

    Trace(LOGGER, "Poll::OnAcceptSocketReceive - data fetched", {
      { "fd", fd },
      { "port", port },
      { "bytes_fetched", (int)result }
    });

    if (result > 0) {
      request.append(_buffer.begin(), _buffer.begin() + result);

      Trace(LOGGER, "Poll::OnAcceptSocketReceive - data appended", {
        { "fd", fd },
        { "port", port }
      });
    } else if (shouldRetry) {
      Trace(LOGGER, "Poll::OnAcceptSocketReceive - flag socket for retry", {
        { "fd", fd },
        { "port", port }
      });

      _socketsToRetry.insert(fd);
      break;
    } else {
      Trace(LOGGER, "Poll::OnAcceptSocketReceive - flag socket for dispose", {
        { "fd", fd },
        { "port", port }
      });

      _socketsToDispose.insert(fd);
      break;
    }
  }

  Trace(LOGGER, "Poll::OnAcceptSocketReceive - request data read complete", {
    { "fd", fd },
    { "port", port },
    { "request_size", (int)request.size() }
  });
  
  if (request.size()) {
    auto ipAddress = acceptSocket->GetRemoteAddress();
    string response = listener.TcpMessageReceived(request, ipAddress, port);
    bool shouldRetry = true;
    int retryCount = 0;

    // For writes, we are retrying in a tight loop instead of going
    // back to poll() like we do for reads.  
    // As a result we set an upper bound for retries.
    //
    // This should eventually be re-written using some sort of write queue.
    while (shouldRetry && retryCount++ < 1000) {
      shouldRetry = acceptSocket->Send(response).Handle(
        [](auto r) { return false; },
        [&](auto err) { 
          Error(LOGGER, err, {{ "fd", acceptSocket->FileDescriptor() }}); 
          return err.ShouldRetry();
        }
      );
    }
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
        acceptSocket->CloseSocket().Handle(
          [](auto r) { return r; },
          [&](auto err) { 
            Error(LOGGER, err, {{ "fd", it->fd }}); 
            return 0;
          }
        );
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