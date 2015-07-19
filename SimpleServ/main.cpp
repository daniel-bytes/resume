#include <iostream>
#include <string>
#include "TcpServer.h"
#include "HttpResponseMessage.h"

class Listener 
	: public TcpMessageListener
{
public:
	virtual void MessageReceived(const std::string &msg, const TcpMessageSender &sender)
	{
		std::cout << msg.size() << " bytes received!\n" << msg << "\n";

		HttpResponseMessage response(Http::StatusCode_OK, "text/html", "<html><head><title>TEST!!</title></head><body><h1>Testing 123!!!</h1></body></html>");

		auto result = sender.Send(response.GetBytes());

		std::cout << "Response sent!" << std::endl;
	}
};

int main() {
	Listener listener;
	TcpServer server;
	server.Connect(3000, listener);

	return 0;
}