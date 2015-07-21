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

		Http::Headers headers;
		Http::StatusCode status;
		std::string contentType;
		std::string output;

		if (message.getPath() == "/") {
			std::ifstream f("./public/index.html");
			output = std::string((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
			f.close();
			status = Http::StatusCode_OK;
			contentType = "text/html";
		}
		else if (message.getPath() == "/style.css") {
			std::ifstream f("./public/style.css");
			output = std::string((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
			f.close();
			status = Http::StatusCode_OK;
			contentType = "text/css";
		}
		else {
			output = "File not found";
			status = Http::StatusCode_NotFound;
			contentType = "text/plain";
		}

		HttpResponseMessage response(status, contentType, headers, output);
		
		return response;
	}
};

int main() {
	AppServer server;
	server.Connect(3000);

	return 0;
}