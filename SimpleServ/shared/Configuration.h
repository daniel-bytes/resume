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
  /** The configured HTTP port **/
  int HttpServerPort() const { return _httpServerPort; }

  /** The optional configured HTTPS port **/
  int HttpsServerPort() const { return _httpsServerPort; }

  /** If true, files to be served from disk should be cached in memory **/
  bool CacheFiles() const { return _cacheFiles; }

  /** If true, HTML templates to be served from disk should be cached in memory **/
  bool CacheTemplates() const { return _cacheTemplates; }

  /** If true, address should be shown in the output HTML **/
  bool ShowAddress() const { return _showAddress; }

  /** If true, projects should be shown in the output HTML **/
  bool ShowProjects() const { return _showProjects; }

  /** If true, SSL should be enabled and accessed via the configured HttpsServerPort **/
  bool SslEnabled() const { return _sslEnabled; }

  /** The current version of the application **/
  const std::string& AppVersion() const { return _appVersion; }

  /** The OpenSSL certificate file path on the local disk **/
  const std::string& SslCertFilePath() const { return _sslCertFilePath; }

  /** The OpenSSL private key file path on the local disk **/
  const std::string& SslKeyFilePath() const { return _sslKeyFilePath; }

  /** The static string "true" **/
  static const std::string& True() { return _true; }

  /** The static string "false" **/
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