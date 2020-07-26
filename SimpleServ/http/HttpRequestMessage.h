#ifndef __HTTPREQUESTMESSAGE_H__
#define __HTTPREQUESTMESSAGE_H__

#include "Http.h"

#include <iostream>
#include <string>
#include <optional>

class HttpRequestMessage
{
public:
	HttpRequestMessage(const std::string &buffer, const std::optional<std::string> &ipAddress);

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

public:
	std::optional<std::string> GetRemoteAddress() const;

private:
	std::string _method;
	std::string _path;
	std::string _httpVersion;
	Http::Headers _headers;
	std::string _body;
	std::optional<std::string> _ipAddress;
};


#endif //__HTTPREQUESTMESSAGE_H__