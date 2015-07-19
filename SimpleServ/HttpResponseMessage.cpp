#include "HttpResponseMessage.h"
#include <sstream>


HttpResponseMessage::HttpResponseMessage()
{
}

HttpResponseMessage::HttpResponseMessage(Http::StatusCode statusCode, const std::string &contentType, const std::string &body)
	: _statusCode(statusCode), _contentType(contentType), _body(body)
{
}

std::string 
HttpResponseMessage::GetBytes() const
{
	std::ostringstream oss;
	auto status = Http::StatusDescriptions::Get(_statusCode);

	oss << "HTTP/" << DEFAULT_HTTP_VERSION << " " << (int)_statusCode << " " << status << "\n";
	oss << "Content-Type: " << _contentType << "\n";
	oss << "Content-Length: " << _body.size() << "\n\n";
	oss << _body;

	return oss.str();
}