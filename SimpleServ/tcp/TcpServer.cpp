#include "TcpServer.h"
#include "TcpError.h"
#include "Poll.h"
#include "shared/Logger.h"

#define LOGGER "TcpServer"

using namespace Logger::NdJson;

// Implementation
TcpServer::TcpServer(const Configuration &config)
	: _httpPort(config.HttpServerPort())
{
	if (config.SslEnabled()) {
		Info(LOGGER, "SSL enabled, initializing OpenSSL", { 
			{ "cert_file_path", config.SslCertFilePath() },
			{ "key_file_path", config.SslKeyFilePath() }
		});
		_sslConfig = Ssl::IntializeSsl(config);
	}
}

TcpServer::~TcpServer(void)
{
  if (_sslConfig.Enabled()) {
		Info(LOGGER, "SSL enabled, terminating OpenSSL", {});
		_sslConfig = Ssl::TerminateSsl(_sslConfig);
	}
}

void
TcpServer::BlockingListen(TcpMessageListener &listener) {
	Info(LOGGER, "Begin listening", { 
		{ "http_port", _httpPort }, 
		{ "https_port", _sslConfig.Enabled() ? _sslConfig.Port() : 0 } 
	});

	Poll poll(_httpPort, _sslConfig);
	poll.BlockingPoll(listener);

	Info(LOGGER, "End listening", { 
		{ "http_port", _httpPort }, 
		{ "https_port", _sslConfig.Enabled() ? _sslConfig.Port() : 0 } 
	});
}
