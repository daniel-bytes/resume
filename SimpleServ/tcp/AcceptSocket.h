#ifndef __ACCEPTSOCKET_H__
#define __ACCEPTSOCKET_H__

#include "Socket.h"
#include "Typedefs.h"
#include <string>
#include <optional>

class AcceptSocket
  : public Socket
{
public:
  AcceptSocket(const socket_t listenSocket = -1);
  AcceptSocket(const AcceptSocket &socket);

public:
  std::optional<std::string> GetRemoteAddress() const;
};

#endif //__ACCEPTSOCKET_H__