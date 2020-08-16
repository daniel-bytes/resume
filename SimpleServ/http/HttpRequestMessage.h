#ifndef __HTTPREQUESTMESSAGE_H__
#define __HTTPREQUESTMESSAGE_H__

#include "Http.h"

#include <iostream>
#include <string>
#include <optional>

/**
 * A data model representing a parsed HTTP request
 */
class HttpRequestMessage
{
public:
	typedef std::string RequestId;
	typedef std::string IpAddress;

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

	const Http::Query& GetQuery() const {
		return _query;
	}

	const std::string& GetBody() const {
		return _body;
	}

	const RequestId& GetRequestId() const {
		return _requestId;
	}

	const IpAddress& GetIpAddress() const {
		return _ipAddress;
	}

public:
	static RequestId GenerateRequestId();

private:
	std::string _method;
	std::string _path;
	std::string _httpVersion;
	Http::Headers _headers;
	Http::Query _query;
	std::string _body;
	RequestId _requestId;
	IpAddress _ipAddress;
};


#endif //__HTTPREQUESTMESSAGE_H__