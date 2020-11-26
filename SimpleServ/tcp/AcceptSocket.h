#ifndef __ACCEPTSOCKET_H__
#define __ACCEPTSOCKET_H__

#include "Socket.h"
#include "Typedefs.h"
#include <array>
#include <string>
#include <optional>

#define ACCEPT_BUFFER_SIZE 80

/**
 * A Socket that accepts a connection from a ListenSocket
 */
class AcceptSocket
  : public Socket
{
public:
  AcceptSocket(const socket_t listenSocket = -1);
  AcceptSocket(const AcceptSocket &socket);

public:
  virtual size_t Recv(std::array<char, ACCEPT_BUFFER_SIZE>& buffer);
  std::optional<std::string> GetRemoteAddress() const;
};

#endif //__ACCEPTSOCKET_H__