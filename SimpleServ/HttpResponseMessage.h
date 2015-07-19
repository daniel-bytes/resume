#ifndef __HTTPMESSAGE_H__
#define __HTTPMESSAGE_H__

#include "HttpStatus.h"

#define DEFAULT_HTTP_VERSION "1.1"

class HttpResponseMessage
{
public:
	HttpResponseMessage();
	HttpResponseMessage(Http::StatusCode statusCode, const std::string &contentType, const std::string &body);

	std::string GetBytes() const;

private:
	Http::StatusCode _statusCode;
	std::string _contentType;
	std::string _body;

};

#endif //__HTTPMESSAGE_H__