#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__

#include <string>
#include <memory>
#include "TcpMessageListener.h"

class TcpServer
{
public:
	class ImplData;

public:
	TcpServer(void);
	~TcpServer(void);

public:
	int Send(const std::string &msg) const;
	void Connect(int port, TcpMessageListener &listener);
	void Disconnect();

private:
	std::unique_ptr<ImplData> _data;
};

#endif //__TCPSERVER_H__