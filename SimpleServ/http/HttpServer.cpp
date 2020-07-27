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
		Log::LogException(LOGGER, ipAddress.value_or("unknown"), httpErr);

		return httpErr.CreateResponse();
	} catch (const std::runtime_error &err) {
		Log::LogException(LOGGER, ipAddress.value_or("unknown"), err);
		
		return HttpResponseMessage(
			Http::StatusCode::InternalServerError,
			Http::ContentTypes::PlainText(),
			{},
			"Internal server error",
			HttpRequestMessage::GenerateRequestId()
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