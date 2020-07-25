#ifndef __HTTPREQUESTMESSAGE_H__
#define __HTTPREQUESTMESSAGE_H__

#include "Http.h"
#include "tcp/IpAddress.h"

#include <iostream>
#include <string>

class HttpRequestMessage
{
public:
	HttpRequestMessage(const std::string &buffer, const IpAddress &ipAddress);

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

	const IpAddress& GetIpAddress() const {
		return _ipAddress;
	}

private:
	std::string _method;
	std::string _path;
	std::string _httpVersion;
	Http::Headers _headers;
	std::string _body;
	IpAddress _ipAddress;
};


#endif //__HTTPREQUESTMESSAGE_H__