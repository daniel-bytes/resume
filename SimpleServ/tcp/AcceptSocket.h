#ifndef __ACCEPTSOCKET_H__
#define __ACCEPTSOCKET_H__

#include "Socket.h"

#include <string>
#include <optional>

class AcceptSocket
  : public Socket
{
public:
  AcceptSocket(int listenSocket);

public:
  std::optional<std::string> GetRemoteAddress() const;
};

#endif //__ACCEPTSOCKET_H__