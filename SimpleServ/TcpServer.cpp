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

// RAII wrappers around Winsock lib objects

class WinSocket
{
public:
	WinSocket(void)
		: _socket(INVALID_SOCKET)
	{
	}

	WinSocket(SOCKET socket)
		: _socket(socket)
	{
	}

	WinSocket(WinSocket &other)
		: _socket(INVALID_SOCKET)
	{
		// transfer ownership
		this->_socket = other._socket;
		other._socket = INVALID_SOCKET;
	}

	~WinSocket(void)
	{
		if (_socket != INVALID_SOCKET) {
			closesocket(_socket);
			_socket = INVALID_SOCKET;
		}
	}

	void Create(addrinfo *address)
	{
		_socket = socket(address->ai_family, address->ai_socktype, address->ai_protocol);

		if (_socket == INVALID_SOCKET) {
			throw NetworkError("socket failed.", WSAGetLastError());
		}
	}

	void Bind(addrinfo *address)
	{
		auto iResult = bind(_socket, address->ai_addr, (int)address->ai_addrlen);

		if (iResult == SOCKET_ERROR) {
			throw NetworkError("bind failed.", iResult);
		}
	}

	void Listen()
	{
		auto iResult = listen(_socket, SOMAXCONN);

		if (iResult == SOCKET_ERROR) {
			throw NetworkError("listen failed.", WSAGetLastError());
		}
	}

	WinSocket Accept()
	{
		SOCKET result = accept(_socket, NULL, NULL);

		if (result == INVALID_SOCKET) {
			throw NetworkError("accept failed.", WSAGetLastError());
		}

		return result;
	}

	std::string Receive(char *buffer, size_t len)
	{
		auto iResult = recv(_socket, buffer, len, 0);

		if (iResult > 0) {
			return std::string(buffer, iResult);
		}
		else if (iResult == SOCKET_ERROR) {
			throw NetworkError("recv failed.", WSAGetLastError());
		}
	}

	int Send(const std::string &msg)
	{
		auto iResult = send(_socket, msg.c_str(), msg.size(), 0);

		if (iResult == SOCKET_ERROR) {
			throw NetworkError("recv failed.", WSAGetLastError());
		}

		return iResult;
	}

private:
	SOCKET _socket;
};

class WinsockLib
{
public:
	WinsockLib(int port)
		: _winsockInitialized(false), _addressinfo(nullptr)
	{
		// init winsock
		auto iResult = WSAStartup(MAKEWORD(2, 2), &_wsaData);

		if (iResult != 0) {
			throw NetworkError("WSAStartup failed.", iResult);
		}

		_winsockInitialized = true;

		// setup address info
		struct addrinfo hints;
		std::ostringstream oss;
		oss << port;

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;

		iResult = getaddrinfo(NULL, oss.str().c_str(), &hints, &_addressinfo);
		if (iResult != 0) {
			throw NetworkError("getaddrinfo failed.", iResult);
		}
	}

	~WinsockLib(void)
	{
		if (_winsockInitialized) {
			WSACleanup();
			_winsockInitialized = false;
		}

		if (_addressinfo != nullptr) {
			freeaddrinfo(_addressinfo);
			_addressinfo = nullptr;
		}
	}

	WinSocket CreateListenSocket(void) {
		WinSocket socket;
		socket.Create(_addressinfo);
		socket.Bind(_addressinfo);
		socket.Listen();
		return socket;
	}

private:
	bool _winsockInitialized;
	WSADATA _wsaData;
	addrinfo *_addressinfo;
};


TcpServer::TcpServer(void)
{
}

TcpServer::~TcpServer(void)
{
}

// Implimentation
void
TcpServer::BlockingListen(int port, TcpMessageListener &listener)
{
	std::cout << "BlockingListen Begin\n";

	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;
	WinsockLib winsock(port);

	WinSocket listenSocket = winsock.CreateListenSocket();
	
	while (true) {
		std::cout << "Accepting connection\n";
		WinSocket acceptSocket = listenSocket.Accept();

		std::string request = acceptSocket.Receive(recvbuf, recvbuflen);
		std::string response = listener.TcpMessageReceived(request);

		acceptSocket.Send(response);
	}

	std::cout << "BlockingListen End\n";
}

#endif