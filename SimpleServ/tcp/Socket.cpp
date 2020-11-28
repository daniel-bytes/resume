#include "Socket.h"
#include "TcpError.h"
#include "shared/Logger.h"

#define LOGGER "Socket"

using namespace Logger::NdJson;

Result<size_t, TcpError>
Socket::Send(const std::string &data)
{
  Trace(LOGGER, "Socket::Send", { { "fd", _socket } });
  AssertValid();

  size_t result = send(_socket, data.c_str(), data.size(), 0);

  if (result < 0) {
    return TcpError("Socket::Send failed", result);
  }

  return result;
}

Result<size_t, TcpError>
Socket::CloseSocket() 
{
  Trace(LOGGER, "Socket::CloseSocket", { { "fd", _socket } });
  
  if (IsActive()) {
    result_t result = close(_socket);
    _socket = -1;

    if (result < 0) {
      return TcpError("CloseSocket failed", errno);
    }
  }

  return 0;
}

void 
Socket::SetReusable() 
{
  Trace(LOGGER, "Socket::SetReusable", { { "fd", _socket } });
  AssertValid();

  const int _on = 1;
  result_t result = setsockopt(_socket, SOL_SOCKET,  SO_REUSEADDR,
                (char *)&_on, sizeof(_on));
  if (result < 0) {
    CloseSocket();
    throw TcpError("Failed to set socket reusable.", errno);
  }
}

void 
Socket::SetNonBlocking() 
{
  Trace(LOGGER, "Socket::SetNonBlocking", { { "fd", _socket } });
  AssertValid();

  result_t flags = fcntl(_socket, F_GETFL, 0);
  if (flags == -1) {
    CloseSocket();
    throw TcpError("Failed to get fcntl flags.", errno);
  }

  flags |= O_NONBLOCK;
  result_t result = fcntl(_socket, F_SETFL, flags) != -1;

  if (result < 0) {
    CloseSocket();
    throw TcpError("Failed to set socket non-blocking.", errno);
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