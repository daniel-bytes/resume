#include "HttpServer.h"
#include "HttpError.h"
#include "Log.h"

HttpResponseMessage
HttpServer::TcpRequestToHttpResponse(const std::string &msg)
{
	try {
		HttpRequestMessage httpRequest(msg);
		
		return HttpMessageReceived(httpRequest);
	} catch (const HttpError &httpErr) {
		Log::Error() << httpErr.what() << std::endl;

		return httpErr.CreateResponse();
	} catch (const std::runtime_error &err) {
		Log::Error() << err.what() << std::endl;
		
		return HttpResponseMessage(
			Http::StatusCode::InternalServerError, 
			"text/plain; charset=utf-8", 
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
HttpServer::TcpMessageReceived(const std::string &msg)
{
	auto httpResponse = TcpRequestToHttpResponse(msg);
	return httpResponse.GetBytes();
}