#include "AcceptSocket.h"
#include "TcpError.h"
#include "shared/Logger.h"
#include <array>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define LOGGER "AcceptSocket"

using namespace Logger::NdJson;

AcceptSocket::AcceptSocket(const socket_t listenSocket) 
{
  Trace(LOGGER, "AcceptSocket::ctor", { { "listen_fd", listenSocket } });

  _socket = accept(listenSocket, NULL, NULL);
  
  if (IsActive()) {
    SetNonBlocking();
  } else if (errno != EWOULDBLOCK) {
    // don't throw here since this is an accept socket
    SocketError(LOGGER, "accept", listenSocket, _socket);
  }
}

AcceptSocket::AcceptSocket(const AcceptSocket &socket)
{
  this->_socket = socket._socket;
}

Result<size_t, TcpError>
AcceptSocket::Recv(std::array<char, ACCEPT_BUFFER_SIZE>& buffer)
{
  auto result = recv(_socket, buffer.begin(), buffer.size(), 0);

  if (result < 0) {
    if (errno == EWOULDBLOCK) {
      return 0;
    }

    return TcpError("Recv failed", result);
  }

  return result;
}

std::optional<std::string>
AcceptSocket::GetRemoteAddress() const
{
  Trace(LOGGER, "AcceptSocket::GetRemoteAddress", { { "fd", _socket } });

  sockaddr_in6 addr;
  socklen_t addr_size = sizeof(sockaddr_in);
  result_t result = getpeername(_socket, (sockaddr*)&addr, &addr_size);

  if (result == 0) {
    std::array<char, INET6_ADDRSTRLEN> buffer = {0};
    const char *pResult = inet_ntop(AF_INET6, &(addr.sin6_addr), buffer.begin(), buffer.size());

    if (pResult != nullptr) {
      return pResult;
    } else {
      SocketError(LOGGER, "inet_ntop", _socket, 0);
    }
  } else {
    SocketError(LOGGER, "getpeername", _socket, result);
  }

  return std::optional<std::string> {};
}
