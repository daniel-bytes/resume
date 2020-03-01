#ifndef __HTTPREQUESTMESSAGE_H__
#define __HTTPREQUESTMESSAGE_H__

#include "Http.h"

#include <iostream>
#include <string>

class HttpRequestMessage
{
public:
	HttpRequestMessage(const std::string &buffer);

public:
	const std::string& GetMethod() const {
		return _method;
	}

	const std::string& GetPath() const {
		return _path;
	}

	const std::string& GetHttpVersion() const {
		return _httpVersion;
	}

	const Http::Headers& GetHeaders() const {
		return _headers;
	}

	const std::string& GetBody() const {
		return _body;
	}

private:
	std::string _method;
	std::string _path;
	std::string _httpVersion;
	Http::Headers _headers;
	std::string _body;
};


#endif //__HTTPREQUESTMESSAGE_H__