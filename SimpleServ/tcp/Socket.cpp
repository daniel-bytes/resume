#include "Socket.h"
#include "TcpError.h"
#include "app/Logger.h"

#define LOGGER "Socket"

using namespace Logger::NdJson;

size_t 
Socket::Send(const std::string &data)
{
  Trace(LOGGER, "Socket::Send");
  AssertValid();

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
Socket::SetReusable() 
{
  Trace(LOGGER, "Socket::SetReusable");
  AssertValid();

  const int _on = 1;
  result_t result = setsockopt(_socket, SOL_SOCKET,  SO_REUSEADDR,
                (char *)&_on, sizeof(_on));
  if (result < 0) {
    CloseSocket();
    throw TcpError("Failed to set socket reusable.");
  }
}

void 
Socket::SetNonBlocking() 
{
  Trace(LOGGER, "Socket::SetNonBlocking");
  AssertValid();

  result_t flags = fcntl(_socket, F_GETFL, 0);
  if (flags == -1) {
    CloseSocket();
    throw TcpError("Failed to get fcntl flags.");
  }

  flags |= O_NONBLOCK;
  result_t result = fcntl(_socket, F_SETFL, flags) != -1;

  if (result < 0) {
    CloseSocket();
    throw TcpError("Failed to set socket non-blocking.");
  } 
}

TcpError
Socket::GetSocketError()
{
  error_code_t error = 0;
  socklen_t errlen = sizeof(error);
  getsockopt(_socket, SOL_SOCKET, SO_ERROR, (void *)&error, &errlen);

  return TcpError(error);
}