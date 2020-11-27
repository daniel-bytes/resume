#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__

#include <string>

#include "Typedefs.h"
#include "TcpMessageListener.h"
#include "Ssl.h"
#include "shared/Configuration.h"

/**
 * TcpServer represents a low-level TCP connection.
 * Typically used as the base by a higher level application protocol server like HTTP.
 */
class TcpServer
{
public:
	TcpServer(const Configuration &config);
	~TcpServer(void);

public:
  /**
   * Begin polling for data on all configured ports, blocking the current thread.
   * Data I/O is processed via the input TcpMessageListener instance.
   */
	void BlockingListen(TcpMessageListener &listener);

private:
  port_t _httpPort;
	SslConfiguration _sslConfig;
};

#endif //__TCPSERVER_H__