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

	/** Callback method for handling HTTP requests, converting to an HTTP response **/
	virtual HttpResponseMessage HttpMessageReceived(const HttpRequestMessage &message);

protected:
  /** Helper for converting HTTP requests to HTTP responses **/
	HttpResponseMessage HandleRequest(const HttpRequestMessage &message);

	/** Returns a 404 FileNotFound error for a given request **/
	HttpResponseMessage FileNotFound(const HttpRequestMessage &message);

	/** Returns a 500 InternalServerError error for a given request **/
	HttpResponseMessage InternalServerError(const HttpRequestMessage &message);

	/** Returns the default headers used in all HTTP responses **/
	Http::Headers GetDefaultHeaders() const;

private:
	Model _model;
	std::unique_ptr<FileServer> _fileServer;
	std::unique_ptr<TemplateParser> _templateParser;
	Configuration _config;
};


#endif //__APPSERVER_H__