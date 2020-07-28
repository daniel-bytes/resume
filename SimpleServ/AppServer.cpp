#include "AppServer.h"
#include "Logger.h"
#include "Utilities.h"

#include <cstdlib>
#include <optional>
#include <numeric>
#include <functional>

#define LOGGER "AppServer"

using namespace Logger::NdJson;

std::string GetExtensionOrDefault(const std::string &filePath)
{
	auto pos = filePath.find_last_of('.');

	if (pos != filePath.npos) {
		return filePath.substr(pos);
	} else {
		return ".txt";
	}
}

void LogRequest(const HttpRequestMessage &request, const HttpResponseMessage &response)
{
	Info(LOGGER, "HTTP request processed", {
		{ "request_id", request.GetRequestId() },
		{ "request_ip_address", request.GetIpAddress() },
		{ "request_method", request.GetMethod() },
		{ "request_path", request.GetPath() },
		{ "request_headers", Utilities::toKeyValuePair(request.GetHeaders()) },
		{ "response_status", static_cast<int>(response.GetStatusCode()) }
	});
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
	: _fileServer(new FileServer)
{
	if (std::getenv("SHOW_ADDRESS")) {
		_model.Set("show_address_section", "true");
	}

	if (std::getenv("SHOW_PROJECTS")) {
		_model.Set("show_projects_section", "true");
	}

	if (std::getenv("CACHE_FILES")) {
		_fileServer.reset(new CachingFileServer);
	}
}

HttpResponseMessage
AppServer::HttpMessageReceived(const HttpRequestMessage &message)
{
	try {
		auto response = ParseRequest(message);
		LogRequest(message, response);

		return response;
	} catch (const std::runtime_error &err) {
		Error(LOGGER, err, {
			{ "request_id", message.GetRequestId() },
			{ "request_ip_address", message.GetIpAddress() }
		});

		auto response = InternalServerError(message);
		LogRequest(message, response);
		
		return response;
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
			auto cachedResult = _fileServer->Get(filePath.value());

			if (cachedResult.has_value()) {
				auto extension = GetExtensionOrDefault(filePath.value());
				auto result = extension == ".html" ? _parser.Apply(cachedResult.value(), _model) : cachedResult.value();
				auto status = Http::StatusCode::OK;
				auto contentType = Http::ContentTypes::GetForExtension(extension)
														.value_or(Http::ContentTypes::PlainText());

				return HttpResponseMessage(
					status, 
					contentType, 
					{},
					result, 
					message.GetRequestId(),
					method.value() == Http::Method::GET);
			}
		}
	}

	return FileNotFound(message);
}

HttpResponseMessage
AppServer::FileNotFound(const HttpRequestMessage &message)
{
	auto status = Http::StatusCode::NotFound;
	auto contentType = Http::ContentTypes::PlainText();
	auto output = "File not found";

	return HttpResponseMessage(
		status, 
		contentType, 
		{},  
		output,
		message.GetRequestId());
}

HttpResponseMessage
AppServer::InternalServerError(const HttpRequestMessage &message)
{
	auto status = Http::StatusCode::InternalServerError;
	auto contentType = Http::ContentTypes::PlainText();
	auto output = "Internal server error";

	return HttpResponseMessage(
		status, 
		contentType, 
		{},
		output,
		message.GetRequestId());
}

