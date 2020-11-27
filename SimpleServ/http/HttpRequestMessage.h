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
  /** Returns the HTTP method **/
	const std::string& GetMethod() const { return _method; }

  /** Returns the HTTP path **/
	const std::string& GetPath() const { return _path; }

  /** Returns the HTTP version **/
	const std::string& GetHttpVersion() const { return _httpVersion; }

  /** Returns the HTTP headers **/
	const Http::Headers& GetHeaders() const { return _headers; }

  /** Returns the HTTP query string parameters **/
	const Http::Query& GetQuery() const { return _query; }

  /** Returns the HTTP request body **/
	const std::string& GetBody() const { return _body; }

  /** Returns the HTTP request ID (used in the X-Request-Id header) **/
	const RequestId& GetRequestId() const { return _requestId; }

  /** Returns the remote request IP address **/
	const IpAddress& GetIpAddress() const { return _ipAddress; }

public:
  /** Generates a new request ID **/
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