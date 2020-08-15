#include "Configuration.h"
#include "Utilities.h"
#include <cstdlib>

#define DEFAULT_PORT 3000

Configuration* Configuration::Global = nullptr;

std::string GetEnvString(const char *env)
{
  auto result = std::getenv(env);
  return result ? std::string(result) : std::string("");
}

void 
Configuration::Initialize(int argc, char *argv[])
{
  Global = new Configuration;
  Global->_true = "true";
  Global->_false = "false";
  Global->_serverPort = DEFAULT_PORT;

  if (argc > 1) {
    Global->_serverPort = Utilities::parseInt(argv[1], DEFAULT_PORT);
  }

  Global->_cacheFiles = GetEnvString("CACHE_FILES") == Global->True();
  Global->_cacheTemplates = GetEnvString("CACHE_TEMPLATES") == Global->True();
  Global->_showAddress = GetEnvString("SHOW_ADDRESS") == Global->True();
  Global->_showProjects = GetEnvString("SHOW_PROJECTS") == Global->True();
  Global->_trace = GetEnvString("TRACE") == Global->True();
  Global->_appVersion = GetEnvString("APP_VERSION");
}


