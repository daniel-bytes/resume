#include "TcpServer.h"
#include "Log.h"
#include "TcpError.h"
#include "Poll.h"

// Implementation
void
TcpServer::BlockingListen(int port, TcpMessageListener &listener)
{
	Log::Info() << "Begin listening on port [" << port << "]" << std::endl;

	Poll poll(port);
	poll.BlockingPoll(listener);

	Log::Info() << "End listening on port [" << port << "]" << std::endl;
}
