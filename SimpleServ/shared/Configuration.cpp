#include "shared/Configuration.h"
#include "shared/Utilities.h"
#include <cstdlib>

#define HTTP_PORT 3080
#define HTTPS_PORT 3443

Configuration::Configuration(int argc, char *argv[])
{
  this->_httpServerPort = HTTP_PORT;
  this->_httpsServerPort = HTTPS_PORT;

  if (argc > 1) {
    this->_httpServerPort = Utilities::ParseInt(argv[1]).value_or(HTTP_PORT);
  }

  if (argc > 2) {
    this->_httpsServerPort = Utilities::ParseInt(argv[2]).value_or(HTTPS_PORT);
  }

  this->_cacheFiles = Utilities::GetEnvVar("CACHE_FILES").value_or("") == _true;
  this->_cacheTemplates = Utilities::GetEnvVar("CACHE_TEMPLATES").value_or("") == _true;
  this->_showAddress = Utilities::GetEnvVar("SHOW_ADDRESS").value_or("") == _true;
  this->_showProjects = Utilities::GetEnvVar("SHOW_PROJECTS").value_or("") == _true;
  this->_sslEnabled = Utilities::GetEnvVar("SSL_ENABLED").value_or("") == _true;
  this->_appVersion = Utilities::GetEnvVar("APP_VERSION").value_or("unknown");
  this->_sslCertFilePath = Utilities::GetEnvVar("SSL_CERT_PATH").value_or("");
  this->_sslKeyFilePath = Utilities::GetEnvVar("SSL_KEY_PATH").value_or("");
}

std::string Configuration::_true = "true";
std::string Configuration::_false = "false";
