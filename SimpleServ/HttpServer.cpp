#include "HttpServer.h"

HttpServer::HttpServer(void)
{
}

HttpServer::~HttpServer(void)
{
}

void
HttpServer::Connect(int port)
{
	_server.Connect(port, *this);
}

void 
HttpServer::Disconnect(void)
{
	_server.Disconnect();
}

void
HttpServer::TcpMessageReceived(const std::string &msg)
{
	HttpRequestMessage httpRequest(msg);
	HttpResponseMessage httpResponse = this->HttpMessageReceived(httpRequest);
	std::string tcpResponse = httpResponse.GetBytes();
	
	_server.Send(tcpResponse);
}