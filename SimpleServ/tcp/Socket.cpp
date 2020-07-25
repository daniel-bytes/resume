#include "Socket.h"
#include "TcpError.h"
#include "Log.h"

size_t 
Socket::Send(const std::string &data)
{
  return send(_socket, data.c_str(), data.size(), 0);
}

void 
Socket::CloseSocket() 
{
  if (IsActive()) {
    close(_socket);
    _socket = -1;
  }
}

void 
Socket::SetReusable() {
  int result = setsockopt(_socket, SOL_SOCKET,  SO_REUSEADDR,
                (char *)&_on, sizeof(_on));
  if (result < 0) {
    CloseSocket();
    throw TcpError("Failed to set socket reusable.");
  }
}

void 
Socket::SetNonBlocking() {
  int flags = fcntl(_socket, F_GETFL, 0);
  if (flags == -1) {
    CloseSocket();
    throw TcpError("Failed to get fcntl flags.");
  }

  flags |= O_NONBLOCK;
  int result = fcntl(_socket, F_SETFL, flags) != -1;

  if (result < 0) {
    CloseSocket();
    throw TcpError("Failed to set socket non-blocking.");
  } 
}