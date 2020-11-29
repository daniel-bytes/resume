#include "html/AppServer.h"
#include "shared/Configuration.h"
#include "shared/Logger.h"
#include "shared/Utilities.h"

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
		{ "request_port", request.GetPort() },
		{ "request_id", request.GetRequestId() },
		{ "request_ip_address", request.GetIpAddress() },
		{ "request_method", request.GetMethod() },
		{ "request_path", request.GetPath() },
		{ "request_headers", Utilities::ToKeyValuePair(request.GetHeaders()) },
		{ "request_query", Utilities::ToKeyValuePair(request.GetQuery()) },
		{ "response_status", static_cast<int>(response.GetStatusCode()) }
	});
}

std::optional<std::string> GetFilePath(const std::string &urlPath)
{
	if (urlPath.size() == 0 || urlPath[0] != '/') {
		return std::optional<std::string> { };
	} else if (urlPath == "/" || urlPath == "/resume") {
		return "./public/index.html";
	} else {
		return std::string("./public") + urlPath;
	}
}

AppServer::AppServer(const Configuration &config)
	: _config(std::move(config)),
	  _fileServer(new FileServer),
		_templateParser(new TemplateParser),
		HttpServer(config)
{
	if (_config.CacheFiles()) {
		_fileServer.reset(new CachingFileServer);
	}

	if (_config.CacheTemplates()) {
		_templateParser.reset(new CachingTemplateParser);
	}

	if (_config.ShowAddress()) {
		_model.Set("show_address_section", Configuration::True());
	}

	if (_config.ShowProjects()) {
		_model.Set("show_projects_section", Configuration::True());
	}
}

HttpResponseMessage
AppServer::HttpMessageReceived(const HttpRequestMessage &message)
{
	Trace(LOGGER, "AppServer::HttpMessageReceived");

	try {
		auto response = HandleRequest(message);
		LogRequest(message, response);

		Trace(LOGGER, "AppServer::HttpMessageReceived end");
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
AppServer::HandleRequest(const HttpRequestMessage &message)
{
	Trace(LOGGER, "AppServer::HandleRequest");

	auto path = message.GetPath();
	auto method = Http::Methods::Get(message.GetMethod());

	if (method.has_value() && 
			(method.value() == Http::Method::GET || method.value() == Http::Method::HEAD)
	) {
		auto upgradedRequest = MaybeUpgradeRequest(message);
		if (upgradedRequest.has_value()) return upgradedRequest.value();

		auto filePath = GetFilePath(path);

		if (filePath.has_value()) {
			auto fileContents = _fileServer->Get(filePath.value());

			if (fileContents.has_value()) {
				auto extension = GetExtensionOrDefault(filePath.value());
				auto result = fileContents.value();
				auto status = Http::StatusCode::OK;
				auto contentType = Http::ContentTypes::GetForExtension(extension)
														.value_or(Http::ContentTypes::PlainText());

				if (extension == ".html") {
					result = _templateParser->Apply(fileContents.value(), _model);
				}

				Trace(LOGGER, "AppServer::HandleRequest end");

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

	Trace(LOGGER, "AppServer::HandleRequest end - FileNotFound");
	return FileNotFound(message);
}

HttpResponseMessage
AppServer::Redirect(
	const HttpRequestMessage &message, 
	const std::string &location,
	const Http::Headers &additionalHeaders
) {
	auto status = Http::StatusCode::MovedPermanently;
	auto contentType = Http::ContentTypes::PlainText();
	auto output = Http::StatusDescriptions::Get(status);
	Http::Headers headers(additionalHeaders);
	headers["Location"] = location;

	return HttpResponseMessage(
		status, 
		contentType, 
		headers,
		output,
		message.GetRequestId()
	);
}

HttpResponseMessage
AppServer::FileNotFound(
	const HttpRequestMessage &message,
	const Http::Headers &additionalHeaders
) {
	auto status = Http::StatusCode::NotFound;
	auto contentType = Http::ContentTypes::PlainText();
	auto output = Http::StatusDescriptions::Get(status);

	return HttpResponseMessage(
		status, 
		contentType, 
		additionalHeaders,  
		output,
		message.GetRequestId());
}

HttpResponseMessage
AppServer::InternalServerError(
	const HttpRequestMessage &message,
	const Http::Headers &additionalHeaders
) {
	auto status = Http::StatusCode::InternalServerError;
	auto contentType = Http::ContentTypes::PlainText();
	auto output = Http::StatusDescriptions::Get(status);

	return HttpResponseMessage(
		status, 
		contentType, 
		additionalHeaders,
		output,
		message.GetRequestId());
}

std::optional<HttpResponseMessage>
AppServer::MaybeUpgradeRequest(const HttpRequestMessage &message)
{
	if (_config.SslEnabled() && message.GetPort() == _config.HttpServerPort()) {
		auto headers = message.GetHeaders();
		auto i = Http::FindHeader(headers, "Upgrade-Insecure-Requests");

		if (i != headers.end() && i->second == "1") {
			auto redirectTo = "https://" + message.GetHost() + message.GetPath();
			return Redirect(message, redirectTo, { { "Vary", "Upgrade-Insecure-Requests" } });
		}
	}
	
	return std::optional<HttpResponseMessage> {};
}