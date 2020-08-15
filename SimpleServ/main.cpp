#include "AppServer.h"
#include "Configuration.h"
#include "Logger.h"
#include "Utilities.h"

#include <exception>

#define LOGGER "main"
#define DEFAULT_PORT 3000

using namespace Logger::NdJson;

void unexpectedHandler() {
	Error(LOGGER, "An unhandled exception occured, exiting process");
	std::abort();
}

int main(int argc, char *argv[]) {
	std::set_terminate(unexpectedHandler);

	try {
		Configuration::Initialize(argc, argv);
		
		AppServer server;
		server.BlockingListen(Configuration::Global->ServerPort());

		return 0;
	} catch(std::exception &exc) {
		Error(LOGGER, exc);
		return 1;
	}
}