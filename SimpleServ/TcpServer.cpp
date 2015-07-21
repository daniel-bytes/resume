#include "TcpServer.h"
#include "NetworkError.h"
#include <sstream>
#include <iostream>

#if _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

#include <windows.h>

#define DEFAULT_BUFLEN 1000000 // 1MB

class TcpServer::ImplData
{
public:
	bool initialized;
	WSADATA wsaData;
	SOCKET listensocket;
	SOCKET acceptsocket;

	ImplData()
		: initialized(false), listensocket(INVALID_SOCKET), acceptsocket(INVALID_SOCKET)
	{
	}

	~ImplData() {}
};


// 

TcpServer::TcpServer(void)
	: _data(new TcpServer::ImplData())
{
}

TcpServer::~TcpServer(void)
{
	Disconnect();
}

int
TcpServer::Send(const std::string &msg) const
{
	return send(_data->acceptsocket, msg.c_str(), msg.size(), 0);
}

void
TcpServer::Connect(int port, TcpMessageListener &listener)
{
	std::cout << "Connect Begin\n";

	int iResult;
	struct addrinfo *result = NULL;
	struct addrinfo hints;

	int iSendResult;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	std::ostringstream oss;
	oss << port;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &_data->wsaData);
	if (iResult != 0) {
		throw NetworkError("WSAStartup failed.", iResult);
	}

	_data->initialized = true;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, oss.str().c_str(), &hints, &result);
	if (iResult != 0) {
		throw NetworkError("getaddrinfo failed.", iResult);
	}

	// Create a SOCKET for connecting to server
	_data->listensocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (_data->listensocket == INVALID_SOCKET) {
		throw NetworkError("socket failed.", WSAGetLastError());
	}

	// Setup the TCP listening socket
	iResult = bind(_data->listensocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		throw NetworkError("bind failed.", iResult);
	}

	freeaddrinfo(result);

	iResult = listen(_data->listensocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		throw NetworkError("listen failed.", WSAGetLastError());
	}

	// Main receive loop
	while (true) {
		std::cout << "Accepting connection\n";
		_data->acceptsocket = accept(_data->listensocket, NULL, NULL);

		if (_data->acceptsocket == INVALID_SOCKET) {
			throw NetworkError("accept failed.", WSAGetLastError());
		}

		iResult = recv(_data->acceptsocket, recvbuf, recvbuflen, 0);

		if (iResult > 0) {
			std::string message(recvbuf, iResult);
			listener.TcpMessageReceived(message);
		}
		else if (iResult == SOCKET_ERROR) {
			throw NetworkError("recv failed.", WSAGetLastError());
		}

		closesocket(_data->acceptsocket);
		_data->acceptsocket = INVALID_SOCKET;
	}
	std::cout << "Accept loop end\n";

	Disconnect();
}

void 
TcpServer::Disconnect()
{
	if (_data->initialized) {
		if (_data->listensocket != INVALID_SOCKET) {
			closesocket(_data->listensocket);
			_data->listensocket = INVALID_SOCKET;
		}

		if (_data->acceptsocket != INVALID_SOCKET) {
			closesocket(_data->acceptsocket);
			_data->acceptsocket = INVALID_SOCKET;
		}

		WSACleanup();

		_data->initialized = false;
	}
}
#endif