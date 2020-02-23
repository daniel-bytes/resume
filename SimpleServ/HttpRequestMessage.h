#ifndef __HTTPREQUESTMESSAGE_H__
#define __HTTPREQUESTMESSAGE_H__

#include "HttpStatus.h"
#include <iostream>
#include <string>

class HttpRequestMessage
{
public:
	HttpRequestMessage(const std::string &buffer);

public:
	Http::StatusCode getStatusCode(void) const {
		return _status;
	}

	const std::string& getMethod(void) const {
		return _method;
	}

	const std::string& getPath(void) const {
		return _path;
	}

	const std::string& getHttpVersion(void) const {
		return _httpVersion;
	}

	const Http::Headers& getHeaders(void) const {
		return _headers;
	}

	const std::string& getBody(void) const {
		return _body;
	}

private:
	Http::StatusCode _status;
	std::string _method;
	std::string _path;
	std::string _httpVersion;
	Http::Headers _headers;
	std::string _body;
};


#endif //__HTTPREQUESTMESSAGE_H__