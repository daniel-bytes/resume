#include "ListenSocket.h"
#include "TcpError.h"

#include <cstring>

void 
ListenSocket::CreateSocket() {
  _socket = socket(AF_INET6, SOCK_STREAM, 0);
  
  if (_socket < 0) {
    throw TcpError("Failed to create socket.");
  }
}

void 
ListenSocket::SocketBind() {
  _addr.sin6_family = AF_INET6;
  memcpy(&_addr.sin6_addr, &in6addr_any, sizeof(in6addr_any));
  _addr.sin6_port = htons(_serverPort);

  int result = bind(_socket, (sockaddr*)&_addr, sizeof(_addr));
  if (result < 0) {
    CloseSocket();
    throw TcpError("Failed to bind socket.");
  }
}

void 
ListenSocket::SocketListen() {
  int result = listen(_socket, _listenSize);
  if (result < 0) {
    CloseSocket();
    throw TcpError("Failed to listen on socket.");
  }
}
