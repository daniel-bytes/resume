#ifndef __LISTENSOCKET_H__
#define __LISTENSOCKET_H__

#include "AcceptSocket.h"
#include "Typedefs.h"

class ListenSocket
  : public Socket
{
public:
  ListenSocket(const port_t serverPort)
    : _serverPort(serverPort), _addr {0}
  {
    CreateSocket();
    SetReusable();
    SetNonBlocking();
    SocketBind();
    SocketListen();
  }

  AcceptSocket Accept();

private:
  void CreateSocket();
  void SocketBind();
  void SocketListen();

private:
  port_t _serverPort;
  sockaddr_in6 _addr;
  const size_t _listenSize = 32;
};

#endif //__LISTENSOCKET_H__