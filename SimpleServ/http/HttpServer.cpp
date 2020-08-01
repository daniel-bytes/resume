#include "HttpServer.h"
#include "HttpError.h"
#include "Logger.h"

#define LOGGER "HttpServer"

using namespace Logger::NdJson;

HttpResponseMessage
HttpServer::TcpRequestToHttpResponse(const std::string &msg, const std::optional<std::string> &ipAddress)
{
	Trace(LOGGER, "HttpServer::TcpRequestToHttpResponse");
	
	try {
		HttpRequestMessage httpRequest(msg, ipAddress);
		
		return HttpMessageReceived(httpRequest);
	} catch (const HttpError &httpErr) {
		Error(LOGGER, httpErr, { { "request_ip_address", ipAddress.value_or("unknown") } });

		return httpErr.CreateResponse();
	} catch (const std::runtime_error &err) {
		Error(LOGGER, err, { { "request_ip_address",  ipAddress.value_or("unknown") } });
		
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