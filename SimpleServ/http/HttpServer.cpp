#include "HttpServer.h"

void
HttpServer::BlockingListen(int port)
{
	_server.BlockingListen(port, *this);
}

std::string
HttpServer::TcpMessageReceived(const std::string &msg)
{
	HttpRequestMessage httpRequest(msg);
	HttpResponseMessage httpResponse = this->HttpMessageReceived(httpRequest);
	std::string tcpResponse = httpResponse.GetBytes();
	
	return tcpResponse;
}