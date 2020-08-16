#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include <string>

/**
 * A type that holds typed application-wide configuration,
 * set via environment variables and command line arguments.
 */
class Configuration
{
public:
  int ServerPort() const { return _serverPort; }
  bool CacheFiles() const { return _cacheFiles; }
  bool CacheTemplates() const { return _cacheTemplates; }
  bool ShowAddress() const { return _showAddress; }
  bool ShowProjects() const { return _showProjects; }
  bool Trace() const { return _trace; }
  const std::string& AppVersion() const { return _appVersion; }

  const std::string& True() const { return _true; }
  const std::string& False() const { return _false; }

  static Configuration* Global;
  static void Initialize(int argc, char *argv[]);

private:
  int _serverPort;
  bool _cacheFiles;
  bool _cacheTemplates;
  bool _showAddress;
  bool _showProjects;
  bool _trace;
  std::string _appVersion;

  std::string _true;
  std::string _false;

  static Configuration *_global;
};

#endif // __CONFIGURATION_H__