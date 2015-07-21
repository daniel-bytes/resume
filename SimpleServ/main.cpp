#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "HttpServer.h"

class AppServer 
	: public HttpServer
{
public:
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
			if (message.getPath() == "/") {
				std::ifstream f("./public/index.html");
				output = std::string((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
				f.close();
				status = Http::StatusCode_OK;
				contentType = "text/html; charset=utf-8";
			}
			else if (message.getPath() == "/style.css") {
				std::ifstream f("./public/style.css");
				output = std::string((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
				f.close();
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