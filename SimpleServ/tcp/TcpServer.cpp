#include "TcpServer.h"
#include "Log.h"
#include "TcpError.h"
#include "Poll.h"

#define LOGGER "TcpServer"

// Implementation
void
TcpServer::BlockingListen(int port, TcpMessageListener &listener)
{
	Log::Info(LOGGER) << "Begin listening on port [" << port << "]" << std::endl;

	Poll poll(port);
	poll.BlockingPoll(listener);

	Log::Info(LOGGER) << "End listening on port [" << port << "]" << std::endl;
}
