#include "AppServer.h"
#include "Utilities.h"

#define DEFAULT_PORT 3000

int main(int argc, char *argv[]) {
	int port = DEFAULT_PORT;

	if (argc > 1) {
		port = Utilities::parseInt(argv[1], DEFAULT_PORT);
	}

	AppServer server;
	server.BlockingListen(port);

	return 0;
}