#ifndef __LISTENSOCKET_H__
#define __LISTENSOCKET_H__

#include "AcceptSocket.h"
#include "Typedefs.h"

#include <memory>

/**
 * A Socket that listens to a port
 */
class ListenSocket
  : public Socket
{
public:
  ListenSocket(const port_t serverPort);
  virtual std::unique_ptr<AcceptSocket> Accept();

private:
  void CreateSocket();
  void SocketBind();
  void SocketListen();

private:
  port_t _httpServerPort;
  sockaddr_in6 _addr;
  const size_t _listenSize = 32;
};

#endif //__LISTENSOCKET_H__