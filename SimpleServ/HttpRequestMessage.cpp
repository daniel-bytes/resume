#include "HttpRequestMessage.h"
#include "HttpError.h"
#include <sstream>
#include <algorithm>

enum HttpRequestPart
{
	HttpRequestPart_Start,
	HttpRequestPart_Headers,
	HttpRequestPart_Body
};


HttpRequestMessage::HttpRequestMessage(const std::string &buffer)
{
	HttpRequestPart current = HttpRequestPart_Start;
	std::stringstream stream(buffer);
	std::string line;

	// Basic HTTP parser.  This needs a LOT of refactoring.
	while (std::getline(stream, line, '\n')) {
		if (line.size() && line[line.size() - 1] == '\r') {
			line = line.substr(0, line.size() - 1);
		}

		switch (current) {
		case HttpRequestPart_Start:
			{
				auto remaining = line;

				// Get method
				auto split = remaining.find_first_of(" ");

				if (split == std::string::npos) {
					throw HttpError(Http::StatusCode_BadRequest, "Malformed begin request line, can't find method: '" + line + "'");
				}

				this->_method = remaining.substr(0, split);
				remaining = remaining.substr(split + 1);

				// Get path
				split = remaining.find_first_of(" ");

				if (split == std::string::npos) {
					throw HttpError(Http::StatusCode_BadRequest, "Malformed begin request line, can't find path: '" + line + "'");
				}

				this->_path = remaining.substr(0, split);
				remaining = remaining.substr(split + 1);

				// Get
				split = remaining.find_first_of("/");

				if (split == std::string::npos) {
					throw HttpError(Http::StatusCode_BadRequest, "Malformed begin request line, can't parse HTTP version: '" + line + "'");
				}
				
				auto http = remaining.substr(0, split);

				if (http != "HTTP") {
					throw HttpError(Http::StatusCode_BadRequest, "Malformed begin request line, missing HTTP from header: '" + line + "'");
				}

				auto version = remaining.substr(split + 1);

				this->_httpVersion = version;

				current = HttpRequestPart_Headers;
			}
			break;

		case HttpRequestPart_Headers:
			{
				bool whiteSpacesOnly = std::all_of(line.begin(), line.end(), isspace);

				if (whiteSpacesOnly) {
					current = HttpRequestPart_Body;
				}
				else {
					auto split = line.find_first_of(": ");

					if (split == std::string::npos) {
						throw HttpError(Http::StatusCode_BadRequest, "Malformed header line: '" + line + "'");
					}

					auto key = line.substr(0, split);
					auto value = line.substr(split + 2);
					this->_headers[key] = value;
				}
			}
			break;
		case HttpRequestPart_Body:
			{
				if (this->_body.size() > 0) {
					this->_body += "\n";
				}

				this->_body += line;
			}
			break;
		}
	}
}
