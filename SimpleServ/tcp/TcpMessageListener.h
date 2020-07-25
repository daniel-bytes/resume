#ifndef __TCPMESSAGELISTENER_H__
#define __TCPMESSAGELISTENER_H__

#include "IpAddress.h"
#include <string>

class TcpMessageListener
{
public:
	virtual std::string TcpMessageReceived(const std::string &msg, const IpAddress &ipAddress) = 0;
};

#endif //__TCPMESSAGELISTENER_H__