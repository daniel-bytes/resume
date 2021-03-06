#include "ListenSocket.h"
#include "shared/Logger.h"
#include "TcpError.h"

#include <cstring>

#define LOGGER "ListenSocket"

using namespace Logger::NdJson;

ListenSocket::ListenSocket(const port_t port)
  : _addr {0}
{
  Trace(LOGGER, "ListenSocket::ctor");
  _port = port;
  CreateSocket();
  SetReusable();
  SetNonBlocking();
  SocketBind();
  SocketListen();
}

std::unique_ptr<AcceptSocket> 
ListenSocket::Accept() {
  Trace(LOGGER, "ListenSocket::Accept", { { "fd", _socket } });
  AssertValid();

  return std::unique_ptr<AcceptSocket>(
    new AcceptSocket(this->FileDescriptor(), this->Port())
  );
}

void 
ListenSocket::CreateSocket() {
  Trace(LOGGER, "ListenSocket::CreateSocket");

  _socket = socket(AF_INET6, SOCK_STREAM, 0);
  
  if (_socket < 0) {
    throw TcpError("Failed to create socket.", errno);
  }
}

void 
ListenSocket::SocketBind() {
  Trace(LOGGER, "ListenSocket::SocketBind", { { "fd", _socket } });
  AssertValid();

  _addr.sin6_family = AF_INET6;
  memcpy(&_addr.sin6_addr, &in6addr_any, sizeof(in6addr_any));
  _addr.sin6_port = htons(_port);

  result_t result = bind(_socket, (sockaddr*)&_addr, sizeof(_addr));
  if (result < 0) {
    CloseSocket();
    throw TcpError("Failed to bind socket.", errno);
  }
}

void 
ListenSocket::SocketListen() {
  Trace(LOGGER, "ListenSocket::SocketListen", { { "fd", _socket } });
  AssertValid();

  result_t result = listen(_socket, _listenSize);
  if (result < 0) {
    CloseSocket();
    throw TcpError("Failed to listen on socket.", errno);
  }
}
