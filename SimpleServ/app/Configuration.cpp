#include "app/Configuration.h"
#include "app/Utilities.h"
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
    Global->_serverPort = Utilities::ParseInt(argv[1]).value_or(DEFAULT_PORT);
  }

  Global->_cacheFiles = Utilities::GetEnvVar("CACHE_FILES").value_or("") == Global->True();
  Global->_cacheTemplates = Utilities::GetEnvVar("CACHE_TEMPLATES").value_or("") == Global->True();
  Global->_showAddress = Utilities::GetEnvVar("SHOW_ADDRESS").value_or("") == Global->True();
  Global->_showProjects = Utilities::GetEnvVar("SHOW_PROJECTS").value_or("") == Global->True();
  Global->_trace = Utilities::GetEnvVar("TRACE").value_or("") == Global->True();
  Global->_appVersion = Utilities::GetEnvVar("APP_VERSION").value_or("unknown");
}


