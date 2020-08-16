#include "ListenSocket.h"
#include "app/Logger.h"
#include "TcpError.h"

#include <cstring>

#define LOGGER "ListenSocket"

using namespace Logger::NdJson;

AcceptSocket 
ListenSocket::Accept() {
  Trace(LOGGER, "ListenSocket::Accept");
  AssertValid();

  return AcceptSocket(this->FileDescriptor());
}

void 
ListenSocket::CreateSocket() {
  Trace(LOGGER, "ListenSocket::CreateSocket");

  _socket = socket(AF_INET6, SOCK_STREAM, 0);
  
  if (_socket < 0) {
    throw TcpError("Failed to create socket.");
  }
}

void 
ListenSocket::SocketBind() {
  Trace(LOGGER, "ListenSocket::SocketBind");
  AssertValid();

  _addr.sin6_family = AF_INET6;
  memcpy(&_addr.sin6_addr, &in6addr_any, sizeof(in6addr_any));
  _addr.sin6_port = htons(_serverPort);

  result_t result = bind(_socket, (sockaddr*)&_addr, sizeof(_addr));
  if (result < 0) {
    CloseSocket();
    throw TcpError("Failed to bind socket.");
  }
}

void 
ListenSocket::SocketListen() {
  Trace(LOGGER, "ListenSocket::SocketListen");
  AssertValid();

  result_t result = listen(_socket, _listenSize);
  if (result < 0) {
    CloseSocket();
    throw TcpError("Failed to listen on socket.");
  }
}
