#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "HttpServer.h"

class AppServer 
	: public HttpServer
{
public:
	std::string ReadFile(const std::string &path)
	{
		std::ifstream f(path);
		std::string result((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
		f.close();

		return result;
	}

	virtual HttpResponseMessage HttpMessageReceived(const HttpRequestMessage &message)
	{
		std::cout << "Http request received\n";
		std::cout << message.getPath() << "\n";

		Http::StatusCode status;
		std::string contentType;
		std::string output;
		Http::Headers headers = { { "X-Powered-By", "SimpleServ" } };

		// bare-bones routing: just allow 2 files needed, otherwise 404 (or 500 on runtime error).
		// TODO : make a regex-based routing system that serves dynamic routes with dynamic content or static files.
		// TODO : standard response headers
		// TODO : gzip
		try {
			auto path = message.getPath();

			std::cout << "- Request path: " << path << "\n";

			if (path == "/") {
				output = ReadFile("./public/index.html");
				status = Http::StatusCode_OK;
				contentType = "text/html; charset=utf-8";
			}
			else if (path == "/style.css") {
				output = ReadFile("./public/style.css");
				status = Http::StatusCode_OK;
				contentType = "text/css; charset=utf-8";
			}
			else {
				output = "File not found";
				status = Http::StatusCode_NotFound;
				contentType = "text/plain; charset=utf-8";
			}
		}
		catch (const std::runtime_error &err) {
			std::cout << "Error! " << err.what() << "\n";
			status = Http::StatusCode_InternalServerError;
			contentType = "text/plain";
			output = "Internal server error";
		}

		HttpResponseMessage response(status, contentType, headers, output);
		
		return response;
	}
};

int main() {
	AppServer server;
	server.BlockingListen(3000);

	return 0;
}