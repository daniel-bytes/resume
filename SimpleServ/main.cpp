#include "html/AppServer.h"
#include "shared/Configuration.h"
#include "shared/Logger.h"
#include "shared/Utilities.h"

#include <exception>

#define LOGGER "main"

using namespace Logger::NdJson;

void unexpectedHandler() {
	Error(LOGGER, "An unhandled exception occured, exiting process");
	std::abort();
}

int main(int argc, char *argv[]) {
	std::set_terminate(unexpectedHandler);

	try {
		AppServer server(Configuration(argc, argv));

		server.BlockingListen();

		return 0;
	} catch(std::exception &exc) {
		Error(LOGGER, exc);
		return 1;
	}
}