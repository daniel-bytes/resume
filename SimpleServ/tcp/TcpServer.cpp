#include "TcpServer.h"
#include "TcpError.h"
#include "Poll.h"
#include "shared/Logger.h"

#define LOGGER "TcpServer"

using namespace Logger::NdJson;

// Implementation
TcpServer::TcpServer(const Configuration &config)
	: _sslContext({ false, 0, nullptr }), _httpPort(config.HttpServerPort())
{
	if (config.SslEnabled()) {
		Info(LOGGER, "SSL enabled, initializing OpenSSL", { 
			{ "cert_file_path", config.SslCertFilePath() },
			{ "key_file_path", config.SslKeyFilePath() }
		});
		_sslContext = Ssl::IntializeSsl(config);
	}
}

TcpServer::~TcpServer(void)
{
  if (_sslContext.enabled) {
		Info(LOGGER, "SSL enabled, terminating OpenSSL", {});
		Ssl::TerminateSsl(_sslContext);
	}
}

void
TcpServer::BlockingListen(TcpMessageListener &listener) {
	Info(LOGGER, "Begin listening", { 
		{ "http_port", _httpPort }, 
		{ "https_port", _sslContext.enabled ? _sslContext.port : 0 } 
	});

	Poll poll(_httpPort, _sslContext);
	poll.BlockingPoll(listener);

	Info(LOGGER, "End listening", { 
		{ "http_port", _httpPort }, 
		{ "https_port", _sslContext.enabled ? _sslContext.port : 0 } 
	});
}
