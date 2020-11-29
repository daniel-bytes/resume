#ifndef __HTTPREQUESTMESSAGE_H__
#define __HTTPREQUESTMESSAGE_H__

#include "Http.h"

#include <iostream>
#include <string>
#include <optional>

#include "tcp/Typedefs.h"

/**
 * A data model representing a parsed HTTP request
 */
class HttpRequestMessage
{
public:
	typedef std::string RequestId;
	typedef std::string IpAddress;

public:
	HttpRequestMessage(
		const std::string &buffer, 
		const std::optional<std::string> &ipAddress,
		const port_t port
	);

public:
  /** Returns the HTTP host header value **/
	const std::string& GetHost() const { return _host; }
	
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

  /** Returns the port the HTTP request originated from **/
	const port_t GetPort() const { return _port; }

public:
  /** Generates a new request ID **/
	static RequestId GenerateRequestId();

private:
	std::string _host;
	std::string _method;
	std::string _path;
	std::string _httpVersion;
	Http::Headers _headers;
	Http::Query _query;
	std::string _body;
	RequestId _requestId;
	IpAddress _ipAddress;
	port_t _port;
};


#endif //__HTTPREQUESTMESSAGE_H__