#ifndef __HTTPSERVER_H__
#define __HTTPSERVER_H__

#include <memory>
#include "tcp/TcpServer.h"
#include "tcp/TcpMessageListener.h"
#include "HttpResponseMessage.h"
#include "HttpRequestMessage.h"

/**
 * Represents an HTTP listener server.
 * Based on TcpServer, it accepts incoming TCP connections, 
 * parses the incoming data as HTTP and returns HTTP responses.
 */
class HttpServer
	: protected TcpMessageListener
{
public:
	void BlockingListen(int port);

protected:
	std::string TcpMessageReceived(const std::string &msg, const IpAddress &ipAddress);
	virtual HttpResponseMessage HttpMessageReceived(const HttpRequestMessage &message) = 0;

	HttpResponseMessage TcpRequestToHttpResponse(const std::string &msg, const IpAddress &ipAddress);
	
protected:
	TcpServer _server;
};

#endif //__HTTPSERVER_H__