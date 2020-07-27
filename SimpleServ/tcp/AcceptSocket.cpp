#include "AcceptSocket.h"
#include "TcpError.h"
#include "Log.h"
#include <array>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define LOGGER "AcceptSocket"

AcceptSocket::AcceptSocket(int listenSocket) {
  _socket = accept(listenSocket, NULL, NULL);
  
  if (IsActive()) {
    SetNonBlocking();
  } else if (errno != EWOULDBLOCK) {
    // don't throw here since this is an accept socket
    Log::LogSocketError(LOGGER, "accept", listenSocket, _socket);
  }
}

std::optional<std::string>
AcceptSocket::GetRemoteAddress() const
{
  sockaddr_in6 addr;
  socklen_t addr_size = sizeof(sockaddr_in);
  int result = getpeername(_socket, (sockaddr*)&addr, &addr_size);

  if (result == 0) {
    std::array<char, INET6_ADDRSTRLEN> buffer = {0};
    const char *pResult = inet_ntop(AF_INET6, &(addr.sin6_addr), buffer.begin(), buffer.size());

    if (pResult != nullptr) {
      return pResult;
    } else {
      Log::LogSocketError(LOGGER, "inet_ntop", _socket, 0);
    }
  } else {
    Log::LogSocketError(LOGGER, "getpeername", _socket, result);
  }

  return std::optional<std::string> {};
}
