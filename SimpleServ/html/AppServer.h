#ifndef __APPSERVER_H__
#define __APPSERVER_H__

#include <memory>
#include "http/HttpServer.h"
#include "html/Model.h"
#include "html/TemplateParser.h"
#include "html/FileServer.h"
#include "shared/Configuration.h"

/**
 * Application-specific server, built on top of HttpServer.
 * Handles our simple resume-serving application.
 */ 
class AppServer 
	: public HttpServer
{
public:
	AppServer(const Configuration &config);
	virtual HttpResponseMessage HttpMessageReceived(const HttpRequestMessage &message);

protected:
	HttpResponseMessage HandleRequest(const HttpRequestMessage &message);
	HttpResponseMessage FileNotFound(const HttpRequestMessage &message);
	HttpResponseMessage InternalServerError(const HttpRequestMessage &message);
	Http::Headers GetDefaultHeaders() const;

private:
	Model _model;
	std::unique_ptr<FileServer> _fileServer;
	std::unique_ptr<TemplateParser> _templateParser;
	Configuration _config;
};


#endif //__APPSERVER_H__