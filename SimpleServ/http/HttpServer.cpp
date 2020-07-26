#include "HttpServer.h"
#include "HttpError.h"
#include "Log.h"

#define LOGGER "HttpServer"

HttpResponseMessage
HttpServer::TcpRequestToHttpResponse(const std::string &msg, const std::optional<std::string> &ipAddress)
{
	try {
		HttpRequestMessage httpRequest(msg, ipAddress);
		
		return HttpMessageReceived(httpRequest);
	} catch (const HttpError &httpErr) {
		Log::Error(LOGGER) << "[" << ipAddress.value_or("unknown") << "] " << httpErr.what() << std::endl;

		return httpErr.CreateResponse();
	} catch (const std::runtime_error &err) {
		Log::Error(LOGGER) << "[" << ipAddress.value_or("unknown") << "] " << err.what() << std::endl;
		
		return HttpResponseMessage(
			Http::StatusCode::InternalServerError,
			Http::ContentTypes::PlainText(),
			Http::DefaultHeaders, 
			"Internal server error"
		);
	}
}

void
HttpServer::BlockingListen(int port)
{
	_server.BlockingListen(port, *this);
}

std::string
HttpServer::TcpMessageReceived(const std::string &msg, const std::optional<std::string> &ipAddress)
{
	auto httpResponse = TcpRequestToHttpResponse(msg, ipAddress);
	return httpResponse.GetBytes();
}