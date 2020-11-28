#ifndef __ACCEPTSOCKET_H__
#define __ACCEPTSOCKET_H__

#include "Socket.h"
#include "Typedefs.h"
#include "shared/Result.h"
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
  /** Receives data from the underlying socket connection **/
  virtual Result<size_t, TcpError> Recv(std::array<char, ACCEPT_BUFFER_SIZE>& buffer);

  /** Gets the remote address of the current socket connection **/
  std::optional<std::string> GetRemoteAddress() const;
};

#endif //__ACCEPTSOCKET_H__