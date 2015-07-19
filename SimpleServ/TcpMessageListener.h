#ifndef __TCPMESSAGELISTENER_H__
#define __TCPMESSAGELISTENER_H__

#include <string>
#include "TcpMessageSender.h"

class TcpMessageListener
{
public:
	virtual void MessageReceived(const std::string &msg, const TcpMessageSender &sender) = 0;
};

#endif //__TCPMESSAGELISTENER_H__