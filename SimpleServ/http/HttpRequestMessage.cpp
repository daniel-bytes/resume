#include "HttpRequestMessage.h"
#include "HttpError.h"
#include "app/Logger.h"
#include "app/Utilities.h"
#include <ctime>
#include <random>
#include <sstream>
#include <string>

#define LOGGER "HttpRequestMessage"

using namespace Logger::NdJson;

enum class HttpRequestPart
{
	Start,
	Headers,
	Body
};

HttpRequestMessage::HttpRequestMessage(const std::string &buffer, const std::optional<std::string> &ipAddress)
	: _ipAddress(ipAddress.value_or("unknown")), 
		_requestId(GenerateRequestId())
{
	Trace(LOGGER, "HttpRequestMessage::ctor");

	HttpRequestPart current = HttpRequestPart::Start;
	std::stringstream stream(buffer);
	std::string line;

	// Basic HTTP parser
	while (std::getline(stream, line, '\n')) {
		if (line.size() && line[line.size() - 1] == '\r') {
			line = line.substr(0, line.size() - 1);
		}

		switch (current) {
		case HttpRequestPart::Start:
			{
				auto remaining = line;

				// Get method
				auto split = remaining.find_first_of(" ");

				if (split == std::string::npos) {
					throw HttpError(Http::StatusCode::BadRequest, "Malformed begin request line, can't find method: '" + line + "'");
				}

				this->_method = remaining.substr(0, split);
				remaining = remaining.substr(split + 1);

				// Get path and query string
				split = remaining.find_first_of(" ");

				if (split == std::string::npos) {
					throw HttpError(Http::StatusCode::BadRequest, "Malformed begin request line, can't find path: '" + line + "'");
				}

				auto path = remaining.substr(0, split);
				auto pathSplit = path.find_first_of("?");

				if (pathSplit == std::string::npos) {
					this->_path = path;
				} else {
					this->_path = path.substr(0, pathSplit);
					this->_query = Utilities::FromKeyValuePair(path.substr(pathSplit + 1));
				}

				remaining = remaining.substr(split + 1);

				// Get HTTP version
				split = remaining.find_first_of("/");

				if (split == std::string::npos) {
					throw HttpError(Http::StatusCode::BadRequest, "Malformed begin request line, can't parse HTTP version: '" + line + "'");
				}
				
				auto http = remaining.substr(0, split);

				if (http != "HTTP") {
					throw HttpError(Http::StatusCode::BadRequest, "Malformed begin request line, missing HTTP from header: '" + line + "'");
				}

				auto version = remaining.substr(split + 1);

				this->_httpVersion = version;

				current = HttpRequestPart::Headers;
			}
			break;

		case HttpRequestPart::Headers:
			{
				bool whiteSpacesOnly = std::all_of(line.begin(), line.end(), isspace);

				if (whiteSpacesOnly) {
					current = HttpRequestPart::Body;
				}
				else {
					auto split = line.find_first_of(": ");

					if (split == std::string::npos) {
						throw HttpError(Http::StatusCode::BadRequest, "Malformed header line: '" + line + "'");
					}

					auto key = line.substr(0, split);
					auto value = line.substr(split + 2);
					this->_headers[key] = value;
				}
			}
			break;
		case HttpRequestPart::Body:
			{
				if (this->_body.size() > 0) {
					this->_body += "\n";
				}

				this->_body += line;
			}
			break;
		}
	}

	// check headers
	for (auto header : _headers) {
		auto headerKey = Utilities::ToLowerCase(header.first);

		if (headerKey == "x-forwarded-for") {
			_ipAddress = header.second;
		} else if (headerKey == "x-request-id") {
			_requestId = header.second;
		}
	}
}

HttpRequestMessage::RequestId
HttpRequestMessage::GenerateRequestId()
{
	return std::to_string(std::time(0) ^ std::rand());
}