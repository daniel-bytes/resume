#ifndef __LISTENSOCKET_H__
#define __LISTENSOCKET_H__

#include "Socket.h"

class ListenSocket
  : public Socket
{
public:
  ListenSocket(int serverPort)
    : _serverPort(serverPort), _addr {0}
  {
    CreateSocket();
    SetReusable();
    SetNonBlocking();
    SocketBind();
    SocketListen();
  }

private:
  void CreateSocket();
  void SocketBind();
  void SocketListen();

private:
  int _serverPort;
  sockaddr_in6 _addr;
  const int _listenSize = 32;
};

#endif //__LISTENSOCKET_H__