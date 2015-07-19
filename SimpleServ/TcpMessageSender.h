#ifndef __TCPMESSAGESENDER_H__
#define __TCPMESSAGESENDER_H__

#include <string>

class TcpMessageSender
{
public:
	virtual int Send(const std::string &msg) const = 0;
};

#endif //__TCPMESSAGESENDER_H__