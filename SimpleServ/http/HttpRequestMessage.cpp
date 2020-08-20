#include "HttpRequestMessage.h"
#include "HttpError.h"
#include "app/Logger.h"
#include "app/Utilities.h"
#include <ctime>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#define LOGGER "HttpRequestMessage"

using std::string;
using std::vector;
using std::stringstream;
using std::optional;
using namespace Logger::NdJson;

enum class HttpRequestPart
{
	Start,
	Headers,
	Body
};

HttpRequestMessage::HttpRequestMessage(const string &buffer, const optional<string> &ipAddress)
	: _ipAddress(ipAddress.value_or("unknown")), 
		_requestId(GenerateRequestId())
{
	Trace(LOGGER, "HttpRequestMessage::ctor");

	HttpRequestPart current = HttpRequestPart::Start;
	stringstream stream(buffer);
	string line;

	// Basic HTTP parser
	while (std::getline(stream, line, '\n')) {
		if (line.size() && line[line.size() - 1] == '\r') {
			line = line.substr(0, line.size() - 1);
		}

		switch (current) {
		case HttpRequestPart::Start:
			{
				auto parts = Utilities::Split(line);

				if (parts.size() != 3) {
					throw HttpError(Http::StatusCode::BadRequest, "Malformed begin request line");
				}

				auto http = Utilities::Split(parts[2], '/');

				if (http.size() != 2 || http[0] != "HTTP") {
					throw HttpError(Http::StatusCode::BadRequest, "Malformed begin request line, invalid HTTP version");
				}

				auto pathAndQuery = Utilities::Split(parts[1], '?');

				this->_method = parts[0];
				this->_httpVersion = http[1];
				this->_path = pathAndQuery[0];

				if (pathAndQuery.size() > 1) {
					this->_query = Utilities::FromKeyValuePair(pathAndQuery[1]);
				}

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

					if (split == string::npos) {
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