#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__

#include <string>

#include "Typedefs.h"
#include "TcpMessageListener.h"

/**
 * TcpServer represents a low-level TCP connection.
 * Typically used as the base by a higher level application protocol server like HTTP.
 */
class TcpServer
{
public:
	void BlockingListen(const port_t port, TcpMessageListener &listener);
};

#endif //__TCPSERVER_H__