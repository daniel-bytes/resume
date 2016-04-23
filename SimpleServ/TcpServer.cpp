#include "TcpServer.h"
#include "NetworkError.h"
#include <sstream>
#include <iostream>

#if _WIN32
	#include <winsock2.h>
	#include <ws2tcpip.h>
	#pragma comment(lib, "Ws2_32.lib")
	#include <windows.h>
	
	int lasterror() {
		return WSAGetLastError();
	}
#else
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <netdb.h>
	#include <unistd.h>
	
	typedef int SOCKET;
	#define INVALID_SOCKET -1
	#define SOCKET_ERROR -1
	
	void closesocket(SOCKET s) {
		close(s);
	}
	
	int lasterror() {
		return errno;
	}
#endif 

#define DEFAULT_BUFLEN 1000000 // 1MB

// C++ wrappers around socket C lib objects

class Socket
{
public:
	Socket(void)
		: _socket(INVALID_SOCKET)
	{
	}

	explicit Socket(SOCKET socket)
		: _socket(socket)
	{
	}

	~Socket(void)
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
			throw NetworkError("socket failed.", lasterror());
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
			throw NetworkError("listen failed.", lasterror());
		}
	}

	void Accept(const Socket &listener)
	{
		SOCKET result = accept(listener._socket, NULL, NULL);

		if (result == INVALID_SOCKET) {
			throw NetworkError("accept failed.", lasterror());
		}
		
		_socket = result;
	}

	std::string Receive(char *buffer, size_t len)
	{
		auto iResult = recv(_socket, buffer, len, 0);

		if (iResult > 0) {
			return std::string(buffer, iResult);
		}
		else if (iResult == SOCKET_ERROR) {
			throw NetworkError("recv failed.", lasterror());
		}
		
		std::string r;
		return r;
	}

	int Send(const std::string &msg)
	{
		auto iResult = send(_socket, msg.c_str(), msg.size(), 0);

		if (iResult == SOCKET_ERROR) {
			throw NetworkError("recv failed.", lasterror());
		}

		return iResult;
	}

private:
	SOCKET _socket;
};

class SocketInit
{
public:
	SocketInit(int port)
		: _winsockInitialized(false), _addressinfo(nullptr)
	{
		// init winsock
		int iResult = 0;
#if _WIN32
		iResult = WSAStartup(MAKEWORD(2, 2), &_wsaData);

		if (iResult != 0) {
			throw NetworkError("WSAStartup failed.", iResult);
		}

		_winsockInitialized = true;
#endif
		// setup address info
		struct addrinfo hints = {};
		std::ostringstream oss;
		oss << port;

		//ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;

		iResult = getaddrinfo(NULL, oss.str().c_str(), &hints, &_addressinfo);
		if (iResult != 0) {
			throw NetworkError("getaddrinfo failed.", iResult);
		}
	}

	~SocketInit(void)
	{
#if _WIN32
		if (_winsockInitialized) {
			WSACleanup();
			_winsockInitialized = false;
		}
#endif
		if (_addressinfo != nullptr) {
			freeaddrinfo(_addressinfo);
			_addressinfo = nullptr;
		}
	}
	
	addrinfo* getAddressInfo(void) const {
		return _addressinfo;
	}

private:
#if _WIN32
	WSADATA _wsaData;
#endif
	bool _winsockInitialized;
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
	std::cout << "BlockingListen Begin on port " << port << "\n";

	SocketInit socketlib(port);
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	Socket listenSocket;
	listenSocket.Create(socketlib.getAddressInfo());
	listenSocket.Bind(socketlib.getAddressInfo());
	listenSocket.Listen();
	
	// very simple/naive single-threaded receive loop
	// TODO: switch to async IO (completion ports)
	while (true) {
		std::cout << "Accepting connection\n";
		Socket acceptSocket;
		acceptSocket.Accept(listenSocket);
		
		std::string request = acceptSocket.Receive(recvbuf, recvbuflen);
		std::string response = listener.TcpMessageReceived(request);

		acceptSocket.Send(response);
	}

	std::cout << "BlockingListen End\n";
}
