#ifndef __APPSERVER_H__
#define __APPSERVER_H__

#include "http/HttpServer.h"
#include "html/Model.h"
#include "html/TemplateParser.h"

/**
 * Application-specific server, built on top of HttpServer.
 * Handles our simple resume-serving application.
 */ 
class AppServer 
	: public HttpServer
{
public:
	AppServer();
	virtual HttpResponseMessage HttpMessageReceived(const HttpRequestMessage &message);

protected:
	HttpResponseMessage Get(const HttpRequestMessage &message);
	HttpResponseMessage Get_StyleCss(const HttpRequestMessage &message);
	HttpResponseMessage FileNotFound(const HttpRequestMessage &message);
	HttpResponseMessage InternalServerError(const HttpRequestMessage &message);

protected:
	Http::Headers GetDefaultHeaders() const;
	HttpResponseMessage ParseRequest(const HttpRequestMessage &message);

private:
	Model _model;
	TemplateParser _parser;
	std::string _getCache;
	std::string _getStyleCache;
};


#endif //__APPSERVER_H__