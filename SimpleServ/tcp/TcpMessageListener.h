#ifndef __TCPMESSAGELISTENER_H__
#define __TCPMESSAGELISTENER_H__

#include <optional>
#include <string>

class TcpMessageListener
{
public:
	virtual std::string TcpMessageReceived(const std::string &msg, const std::optional<std::string> &ipAddress) = 0;
};

#endif //__TCPMESSAGELISTENER_H__