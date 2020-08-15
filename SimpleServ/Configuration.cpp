#include "Configuration.h"
#include "Utilities.h"
#include <cstdlib>

#define DEFAULT_PORT 3000

Configuration* Configuration::Global = nullptr;

void 
Configuration::Initialize(int argc, char *argv[])
{
  Global = new Configuration;
  Global->_true = "true";
  Global->_false = "false";
  Global->_serverPort = DEFAULT_PORT;

  if (argc > 1) {
    Global->_serverPort = Utilities::ParseInt(argv[1], DEFAULT_PORT);
  }

  Global->_cacheFiles = Utilities::GetEnvVar("CACHE_FILES") == Global->True();
  Global->_cacheTemplates = Utilities::GetEnvVar("CACHE_TEMPLATES") == Global->True();
  Global->_showAddress = Utilities::GetEnvVar("SHOW_ADDRESS") == Global->True();
  Global->_showProjects = Utilities::GetEnvVar("SHOW_PROJECTS") == Global->True();
  Global->_trace = Utilities::GetEnvVar("TRACE") == Global->True();
  Global->_appVersion = Utilities::GetEnvVar("APP_VERSION");
}


