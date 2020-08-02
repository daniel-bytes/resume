#include "Socket.h"
#include "TcpError.h"
#include "Logger.h"

#define LOGGER "Socket"

using namespace Logger::NdJson;


size_t 
Socket::Send(const std::string &data)
{
  Trace(LOGGER, "Socket::Send");
  size_t result = send(_socket, data.c_str(), data.size(), 0);

  if (result < 0) {
    SocketError(LOGGER, "send", _socket, result);
  }

  return result;
}

void 
Socket::CloseSocket() 
{
  Trace(LOGGER, "Socket::CloseSocket");
  if (IsActive()) {
    close(_socket);
    _socket = -1;
  }
}

void 
Socket::SetReusable() {
  Trace(LOGGER, "Socket::SetReusable");
  int result = setsockopt(_socket, SOL_SOCKET,  SO_REUSEADDR,
                (char *)&_on, sizeof(_on));
  if (result < 0) {
    CloseSocket();
    throw TcpError("Failed to set socket reusable.");
  }
}

void 
Socket::SetNonBlocking() {
  Trace(LOGGER, "Socket::SetNonBlocking");
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