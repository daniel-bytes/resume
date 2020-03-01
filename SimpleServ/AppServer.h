#ifndef __APPSERVER_H__
#define __APPSERVER_H__

#include "http/HttpServer.h"

/**
 * Application-specific server, built on top of HttpServer.
 * Handles our simple resume-serving application.
 */ 
class AppServer 
	: public HttpServer
{
public:
	virtual HttpResponseMessage HttpMessageReceived(const HttpRequestMessage &message);

protected:
	virtual HttpResponseMessage ParseRequest(const HttpRequestMessage &message);
	virtual HttpResponseMessage Get(const HttpRequestMessage &message) const;
	virtual HttpResponseMessage Get_StyleCss(const HttpRequestMessage &message) const;
	virtual HttpResponseMessage FileNotFound(const HttpRequestMessage &message) const;
	virtual HttpResponseMessage InternalServerError(const HttpRequestMessage &message) const;

private:
	Http::Headers GetDefaultHeaders() const;
};


#endif //__APPSERVER_H__