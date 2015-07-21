#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__

#include <string>
#include "TcpMessageListener.h"

class TcpServer
{
public:
	TcpServer(void);
	~TcpServer(void);

public:
	void BlockingListen(int port, TcpMessageListener &listener);
};

#endif //__TCPSERVER_H__