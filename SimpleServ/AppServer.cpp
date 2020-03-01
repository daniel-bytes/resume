#include "AppServer.h"
#include "Log.h"
#include "Utilities.h"

HttpResponseMessage
AppServer::HttpMessageReceived(const HttpRequestMessage &message)
{
	try {
		auto response = ParseRequest(message);

		Log::Info()
			<< message.GetMethod()
			<< " " << message.GetPath()
			<< " - " << static_cast<int>(response.GetStatusCode())
			<< std::endl;

		return response;
	} catch (const std::runtime_error &err) {
		Log::Error() << err.what() << std::endl;
		return InternalServerError(message);
	}
}

HttpResponseMessage 
AppServer::ParseRequest(const HttpRequestMessage &message)
{
	auto path = message.GetPath();

	if (path == "/") {
		return Get(message);
	} else if (path == "/style.css") {
		return Get_StyleCss(message);
	} else {
		return FileNotFound(message);
	}
}

HttpResponseMessage
AppServer::Get(const HttpRequestMessage &message) const
{
	auto status = Http::StatusCode::OK;
	auto contentType = "text/html; charset=utf-8";
	auto output = Utilities::readFileIntoString("./public/index.html");
	
	return HttpResponseMessage(status, contentType, Http::DefaultHeaders, output);
}

HttpResponseMessage
AppServer::Get_StyleCss(const HttpRequestMessage &message) const
{
	auto status = Http::StatusCode::OK;
	auto contentType = "text/css; charset=utf-8";
	auto output = Utilities::readFileIntoString("./public/style.css");

	return HttpResponseMessage(status, contentType, Http::DefaultHeaders, output);
}

HttpResponseMessage
AppServer::FileNotFound(const HttpRequestMessage &message) const
{
	auto status = Http::StatusCode::NotFound;
	auto contentType = "text/plain; charset=utf-8";
	auto output = "File not found";

	return HttpResponseMessage(status, contentType, Http::DefaultHeaders, output);
}

HttpResponseMessage
AppServer::InternalServerError(const HttpRequestMessage &message) const
{
	auto status = Http::StatusCode::InternalServerError;
	auto contentType = "text/plain; charset=utf-8";
	auto output = "Internal server error";

	return HttpResponseMessage(status, contentType, Http::DefaultHeaders, output);
}

