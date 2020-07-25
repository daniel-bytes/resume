#include "AppServer.h"
#include "Log.h"
#include "Utilities.h"

#include <cstdlib>

AppServer::AppServer()
{
	if (std::getenv("SHOW_ADDRESS")) {
		_model.Set("show_address_section", "true");
	}

	if (std::getenv("SHOW_PROJECTS")) {
		_model.Set("show_projects_section", "true");
	}
}

HttpResponseMessage
AppServer::HttpMessageReceived(const HttpRequestMessage &message)
{
	try {
		auto response = ParseRequest(message);

		Log::Info()
		  << "[" << message.GetIpAddress().Text() << "] "
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
	auto method = message.GetMethod();
	auto isGet = method == "GET";
	auto isHead = method == "HEAD";

	if (isGet || isHead) {
		if (path == "/") return Get(message, isGet);
		if (path == "/style.css") return Get_StyleCss(message, isGet);
		if (path == "/robots.txt") return Get_RobotsTxt(message, isGet);
	}

	return FileNotFound(message);
}

HttpResponseMessage
AppServer::Get(const HttpRequestMessage &message, bool includeContent)
{
	auto status = Http::StatusCode::OK;
	auto contentType = "text/html; charset=utf-8";

	if (_getCache.empty()) {
		auto file = Utilities::readFileIntoString("./public/index.html");
		_getCache = _parser.Apply(file, _model);
	}

	return HttpResponseMessage(status, contentType, Http::DefaultHeaders, _getCache, includeContent);
}

HttpResponseMessage
AppServer::Get_StyleCss(const HttpRequestMessage &message, bool includeContent)
{
	auto status = Http::StatusCode::OK;
	auto contentType = "text/css; charset=utf-8";

	if (_getStyleCache.empty()) {
		_getStyleCache = Utilities::readFileIntoString("./public/style.css");
	}

	return HttpResponseMessage(status, contentType, Http::DefaultHeaders, _getStyleCache, includeContent);
}

HttpResponseMessage
AppServer::Get_RobotsTxt(const HttpRequestMessage &message, bool includeContent)
{
	auto status = Http::StatusCode::OK;
	auto contentType = "text/plain; charset=utf-8";

	if (_getRobotsTxtCache.empty()) {
		_getRobotsTxtCache = Utilities::readFileIntoString("./public/robots.txt");
	}

	return HttpResponseMessage(status, contentType, Http::DefaultHeaders, _getRobotsTxtCache, includeContent);
}

HttpResponseMessage
AppServer::FileNotFound(const HttpRequestMessage &message)
{
	auto status = Http::StatusCode::NotFound;
	auto contentType = "text/plain; charset=utf-8";
	auto output = "File not found";

	return HttpResponseMessage(status, contentType, Http::DefaultHeaders, output);
}

HttpResponseMessage
AppServer::InternalServerError(const HttpRequestMessage &message)
{
	auto status = Http::StatusCode::InternalServerError;
	auto contentType = "text/plain; charset=utf-8";
	auto output = "Internal server error";

	return HttpResponseMessage(status, contentType, Http::DefaultHeaders, output);
}

