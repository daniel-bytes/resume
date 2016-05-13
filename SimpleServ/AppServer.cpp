#include <iostream>
#include "AppServer.h"
#include "Utilities.h"


HttpResponseMessage
AppServer::HttpMessageReceived(const HttpRequestMessage &message)
{
	std::cout << "Http request received\n";
	std::cout << message.getPath() << "\n";

	// bare-bones routing: just allow 2 files needed, otherwise 404 (or 500 on runtime error).
	// TODO : make a regex-based routing system that serves dynamic routes with dynamic content or static files.
	// TODO : standard response headers
	// TODO : gzip
	try {
		auto path = message.getPath();

		std::cout << "- Request path: " << path << "\n";

		if (path == "/") {
			return Get(message);
		}
		else if (path == "/style.css") {
			return Get_StyleCss(message);
		}
		else {
			return FileNotFound(message);
		}
	}
	catch (const std::runtime_error &err) {
		std::cout << "Error! " << err.what() << "\n";
		return InternalServerError(message);
	}
}

HttpResponseMessage
AppServer::Get(const HttpRequestMessage &message) const
{
	auto output = Utilities::readFileIntoString("./public/index.html");
	auto status = Http::StatusCode_OK;
	auto contentType = "text/html; charset=utf-8";

	return HttpResponseMessage(status, contentType, GetDefaultHeaders(), output);
}

HttpResponseMessage
AppServer::Get_StyleCss(const HttpRequestMessage &message) const
{
	auto output = Utilities::readFileIntoString("./public/style.css");
	auto status = Http::StatusCode_OK;
	auto contentType = "text/css; charset=utf-8";

	return HttpResponseMessage(status, contentType, GetDefaultHeaders(), output);
}

HttpResponseMessage
AppServer::FileNotFound(const HttpRequestMessage &message) const
{
	auto output = "File not found";
	auto status = Http::StatusCode_NotFound;
	auto contentType = "text/plain; charset=utf-8";

	return HttpResponseMessage(status, contentType, GetDefaultHeaders(), output);
}

HttpResponseMessage
AppServer::InternalServerError(const HttpRequestMessage &message) const
{
	auto status = Http::StatusCode_InternalServerError;
	auto contentType = "text/plain";
	auto output = "Internal server error";

	return HttpResponseMessage(status, contentType, GetDefaultHeaders(), output);
}


Http::Headers
AppServer::GetDefaultHeaders(void) const
{

	Http::Headers headers = { { "X-Powered-By", "SimpleServ" } };
	return headers;
}
