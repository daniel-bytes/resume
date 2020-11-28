#include "HttpServer.h"
#include "HttpError.h"
#include "shared/Logger.h"

#define LOGGER "HttpServer"

using namespace Logger::NdJson;

HttpServer::HttpServer(const Configuration &config)
	: _server(TcpServer(config))
{
}

HttpResponseMessage
HttpServer::TcpRequestToHttpResponse(
	const std::string &msg, 
	const std::optional<std::string> &ipAddress,
	const port_t port
)
{
	Trace(LOGGER, "HttpServer::TcpRequestToHttpResponse");
	
	try {
		HttpRequestMessage httpRequest(msg, ipAddress, port);
		
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
HttpServer::BlockingListen(void)
{
	_server.BlockingListen(*this);
}

std::string
HttpServer::TcpMessageReceived(
	const std::string &msg, 
	const std::optional<std::string> &ipAddress,
	const port_t port
) {
	auto httpResponse = TcpRequestToHttpResponse(msg, ipAddress, port);
	return httpResponse.GetBytes();
}