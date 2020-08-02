#include "AppServer.h"
#include "Logger.h"
#include "Utilities.h"

#include <exception>

#define LOGGER "main"
#define DEFAULT_PORT 3000

using namespace Logger::NdJson;

void unexpectedHandler() {
	Error(LOGGER, "An unhandled exception occured, exiting process");
	std::terminate();
}

int main(int argc, char *argv[]) {
	std::set_unexpected(unexpectedHandler);

	try {
		int port = DEFAULT_PORT;

		if (argc > 1) {
			port = Utilities::parseInt(argv[1], DEFAULT_PORT);
		}

		AppServer server;
		server.BlockingListen(port);

		return 0;
	} catch(std::exception &exc) {
		Error(LOGGER, exc);
		return 1;
	}
}