#ifndef __HTTPMESSAGE_H__
#define __HTTPMESSAGE_H__

#include "Http.h"

#define DEFAULT_HTTP_VERSION "1.1"

class HttpResponseMessage
{
public:
	HttpResponseMessage();
	HttpResponseMessage(Http::StatusCode statusCode, 
											const std::string &contentType,  
											const Http::Headers &headers,
											const std::string &body);

	Http::StatusCode GetStatusCode() const {
		return _statusCode;
	}

	std::string GetBytes() const;

private:
	Http::StatusCode _statusCode;
	std::string _contentType;
	std::map<std::string, std::string> _headers;
	std::string _body;
};

#endif //__HTTPMESSAGE_H__