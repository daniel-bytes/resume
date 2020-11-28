#ifndef __HTTPSERVER_H__
#define __HTTPSERVER_H__

#include <memory>
#include "tcp/TcpServer.h"
#include "tcp/TcpMessageListener.h"
#include "HttpResponseMessage.h"
#include "HttpRequestMessage.h"
#include "shared/Configuration.h"

/**
 * Represents an HTTP listener server.
 * Based on TcpServer, it accepts incoming TCP connections, 
 * parses the incoming data as HTTP and returns HTTP responses.
 */
class HttpServer
	: protected TcpMessageListener
{
public:
  HttpServer(const Configuration &config);

	/**
   * Begin polling for data on all configured ports, blocking the current thread.
   * Data I/O is processed via the input TcpMessageListener instance.
   */
	void BlockingListen(void);

protected:
  /** Virtual function to be overriden by derived classes to handle incoming HTTP requests **/
	virtual HttpResponseMessage HttpMessageReceived(const HttpRequestMessage &message) = 0;

// Protected methods used to implement BlockingListen
protected:
  /** Callback method that receives and processes TCP data from the underlying TcpServer **/
	std::string TcpMessageReceived(
		const std::string &msg, 
		const std::optional<std::string> &ipAddress, 
		const port_t port
	);
  
	/** Converts a TCP request to an HTTP response **/
	HttpResponseMessage TcpRequestToHttpResponse(
		const std::string &msg, 
		const std::optional<std::string> &ipAddress,
		const port_t port
	);
	
protected:
	TcpServer _server;
};

#endif //__HTTPSERVER_H__