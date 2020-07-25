#include "AppServer.h"
#include "Log.h"
#include "Utilities.h"

#include <cstdlib>
#include <optional>

std::string GetExtensionOrDefault(const std::string &filePath)
{
	auto pos = filePath.find_last_of('.');

	if (pos != filePath.npos) {
		return filePath.substr(pos);
	} else {
		return ".txt";
	}
}

std::optional<std::string> GetFilePath(const std::string &urlPath)
{
	if (urlPath.size() == 0 || urlPath[0] != '/') {
		return std::optional<std::string> { };
	} else if (urlPath == "/") {
		return "./public/index.html";
	} else {
		return std::string("./public") + urlPath;
	}
}

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
	auto method = Http::Methods::Get(message.GetMethod());

	if (method.has_value() && 
			(method.value() == Http::Method::GET || method.value() == Http::Method::HEAD)
	) {
		auto filePath = GetFilePath(path);

		if (filePath.has_value()) {
			auto cachedResult = _cache.Get(filePath.value());

			if (cachedResult.has_value()) {
				auto extension = GetExtensionOrDefault(filePath.value());
				auto result = extension == ".html" ? _parser.Apply(cachedResult.value(), _model) : cachedResult.value();
				auto status = Http::StatusCode::OK;
				auto contentType = Http::ContentTypes::GetForExtension(extension)
														.value_or(Http::ContentTypes::PlainText());

				return HttpResponseMessage(status, contentType, Http::DefaultHeaders, result, method.value() == Http::Method::GET);
			}
		}
	}

	return FileNotFound(message);
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

