#include "HttpRequestMessage.h"
#include "HttpError.h"
#include <sstream>

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
		switch (current) {
		case HttpRequestPart_Start:
			{
				auto rest = line;

				// Get method
				auto split = rest.find_first_of(" ");

				if (split == std::string::npos) {
					throw HttpError(Http::StatusCode_BadRequest, "Malformed begin request line, can't find method: " + line);
				}

				this->_method = rest.substr(0, split);
				rest = rest.substr(split + 1);

				// Get path
				split = rest.find_first_of(" ");

				if (split == std::string::npos) {
					throw HttpError(Http::StatusCode_BadRequest, "Malformed begin request line, can't find path: " + line);
				}

				this->_path = rest.substr(0, split);
				rest = rest.substr(split + 1);

				// Get
				split = rest.find_first_of("/");

				if (split == std::string::npos) {
					throw HttpError(Http::StatusCode_BadRequest, "Malformed begin request line, can't parse HTTP version: " + line);
				}

				this->_httpVersion = rest.substr(split + 1);

				current = HttpRequestPart_Headers;
			}
			break;

		case HttpRequestPart_Headers:
			{
				if (line.size() == 0) {
					current = HttpRequestPart_Body;
				}
				else {
					auto split = line.find_first_of(": ");

					if (split == std::string::npos) {
						throw HttpError(Http::StatusCode_BadRequest, "Malformed header line: " + line);
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
