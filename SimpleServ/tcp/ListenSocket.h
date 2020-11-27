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

  /** Accepts a connection to a remote socket **/
  virtual std::unique_ptr<AcceptSocket> Accept();

// Private implementation functions used by the constructor to create a new listen socket
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