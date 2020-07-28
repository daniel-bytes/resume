#include "TcpServer.h"
#include "TcpError.h"
#include "Poll.h"
#include "Logger.h"

#define LOGGER "TcpServer"

using namespace Logger::NdJson;

// Implementation
void
TcpServer::BlockingListen(int port, TcpMessageListener &listener)
{
	Info(LOGGER, "Begin listening", { { "port", port } });

	Poll poll(port);
	poll.BlockingPoll(listener);

	Info(LOGGER, "End listening", { { "port", port } });
}
