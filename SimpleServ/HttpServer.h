#ifndef __HTTPSERVER_H__
#define __HTTPSERVER_H__

#include <memory>
#include "TcpServer.h"
#include "TcpMessageListener.h"
#include "HttpResponseMessage.h"
#include "HttpRequestMessage.h"

class HttpServer
	: protected TcpMessageListener
{
public:
	HttpServer(void);
	virtual ~HttpServer(void);

public:
	void Connect(int port);
	void Disconnect(void);

protected:
	void TcpMessageReceived(const std::string &msg);
	virtual HttpResponseMessage HttpMessageReceived(const HttpRequestMessage &message) = 0;

protected:
	TcpServer _server;
};

#endif //__HTTPSERVER_H__