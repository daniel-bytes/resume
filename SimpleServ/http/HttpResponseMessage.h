#ifndef __HTTPMESSAGE_H__
#define __HTTPMESSAGE_H__

#include "Http.h"
#include "HttpRequestMessage.h"

#define DEFAULT_HTTP_VERSION "1.1"

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

	const Http::StatusCode GetStatusCode() const {
		return _statusCode;
	}

	std::string GetBytes() const;

	const HttpRequestMessage::RequestId& GetRequestId() const {
		return _requestId;
	}

private:
	Http::StatusCode _statusCode;
	std::string _contentType;
	std::map<std::string, std::string> _headers;
	std::string _body;
	std::string _requestId;
	bool _includeBodyInBytes;
};

#endif //__HTTPMESSAGE_H__