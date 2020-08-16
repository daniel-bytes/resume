#include "HttpResponseMessage.h"
#include "app/Logger.h"
#include <sstream>

#define LOGGER "HttpResponseMessage"

using namespace Logger::NdJson;

HttpResponseMessage::HttpResponseMessage()
{
}

HttpResponseMessage::HttpResponseMessage(
	Http::StatusCode statusCode,
	const std::string &contentType,
	const Http::Headers &headers,
	const std::string &body,
	const std::string &requestId,
	bool includeBodyInBytes
) : _statusCode(statusCode),
		_contentType(contentType),
		_headers(headers),
		_body(body),
		_requestId(requestId),
		_includeBodyInBytes(includeBodyInBytes)
{
}

std::string 
HttpResponseMessage::GetBytes() const
{
	Trace(LOGGER, "HttpResponseMessage::GetBytes");

	std::ostringstream oss;
	auto status = Http::StatusDescriptions::Get(_statusCode);

	oss << "HTTP/" << DEFAULT_HTTP_VERSION << " " << static_cast<int>(_statusCode) << " " << status << "\n";
	oss << "Content-Type: "   << _contentType  << "\n";
	oss << "Content-Length: " << _body.size()  << "\n";
	oss << "X-Request-Id: "   << _requestId    << "\n";
	oss << "X-Powered-By: "   <<  "SimpleServ" << "\n";

	for (auto it : _headers) {
		oss << it.first << ": " << it.second << "\n";
	}

	oss << "\n";

	if (_includeBodyInBytes) {
		oss << _body;
	}

	return oss.str();
}
