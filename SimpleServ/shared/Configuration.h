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
  Configuration(int argc, char *argv[]);

public:
  int HttpServerPort() const { return _httpServerPort; }
  int HttpsServerPort() const { return _httpsServerPort; }
  bool CacheFiles() const { return _cacheFiles; }
  bool CacheTemplates() const { return _cacheTemplates; }
  bool ShowAddress() const { return _showAddress; }
  bool ShowProjects() const { return _showProjects; }
  bool SslEnabled() const { return _sslEnabled; }
  const std::string& AppVersion() const { return _appVersion; }
  const std::string& SslCertFilePath() const { return _sslCertFilePath; }
  const std::string& SslKeyFilePath() const { return _sslKeyFilePath; }

  static const std::string& True() { return _true; }
  static const std::string& False() { return _false; }

private:
  int _httpServerPort;
  int _httpsServerPort;
  bool _cacheFiles;
  bool _cacheTemplates;
  bool _showAddress;
  bool _showProjects;
  bool _sslEnabled;
  std::string _appVersion;
  std::string _sslCertFilePath;
  std::string _sslKeyFilePath;

  static std::string _true;
  static std::string _false;
};

#endif // __CONFIGURATION_H__