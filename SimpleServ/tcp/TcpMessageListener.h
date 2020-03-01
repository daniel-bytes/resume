#ifndef __TCPMESSAGELISTENER_H__
#define __TCPMESSAGELISTENER_H__

#include <string>

class TcpMessageListener
{
public:
	virtual std::string TcpMessageReceived(const std::string &msg) = 0;
};

#endif //__TCPMESSAGELISTENER_H__