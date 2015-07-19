#ifndef __TCPSERVERWIN32IMPL_H__
#define __TCPSERVERWIN32IMPL_H__

#include "TcpImpl.h"

#if _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

#include <windows.h>

class Win32TcpImpl
	: public TcpImpl
{
public:
	Win32TcpImpl(void);
	~Win32TcpImpl(void);

	void Connect(int port, TcpMessageListener &listener);
	void Disconnect();

private:
	bool _initialized;
	WSADATA _wsaData;
	SOCKET _listensocket;
	SOCKET _acceptsocket;
};

#endif

#endif //__TCPSERVERWIN32IMPL_H__