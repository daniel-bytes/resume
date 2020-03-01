#ifndef __ACCEPTSOCKET_H__
#define __ACCEPTSOCKET_H__

#include "Socket.h"

class AcceptSocket
  : public Socket
{
public:
  AcceptSocket(int listenSocket);
};

#endif //__ACCEPTSOCKET_H__