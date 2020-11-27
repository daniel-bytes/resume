#ifndef __HTTPMESSAGE_H__
#define __HTTPMESSAGE_H__

#include "Http.h"
#include "HttpRequestMessage.h"

#define DEFAULT_HTTP_VERSION "1.1"

/**
 * A data model representing an outgoing HTTP response
 */
class HttpResponseMessage
{
public:
	HttpResponseMessage();

	HttpResponseMessage(Http::StatusCode statusCode, 
											const std::string &contentType,  
											const Http::Headers &headers,
											const std::string &body,
											const HttpRequestMessage::RequestId &requestId,
											bool includeBodyInBytes = true);

  /** Returns the HTTP response status code **/
	const Http::StatusCode GetStatusCode() const { return _statusCode; }

  /** Serializes the structured HTTP response body to bytes **/
	std::string GetBytes() const;

  /** Returns the ID associated with this response's request **/
	const HttpRequestMessage::RequestId& GetRequestId() const { return _requestId; }

private:
	Http::StatusCode _statusCode;
	std::string _contentType;
	Http::Headers _headers;
	std::string _body;
	std::string _requestId;
	bool _includeBodyInBytes;
};

#endif //__HTTPMESSAGE_H__