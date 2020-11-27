#ifndef __SSL_H__
#define __SSL_H__

#include "AcceptSocket.h"
#include "ListenSocket.h"
#include "Typedefs.h"
#include "shared/Configuration.h"
#include <memory>
#include <string>

/**
 * Opaque pointer to an SSL context.
 */
typedef void* SslContext;

/**
 * Opaque pointer to an SSL connection.
 */
typedef void* SslConnection;

/**
 * Ssl configuration and context wrapper
 */
class SslConfiguration
{
public:
  SslConfiguration(void)
    : _enabled(false), 
      _port(0), 
      _context(nullptr) {};

  SslConfiguration(const Configuration &config, SslContext context)
    : _enabled(config.SslEnabled()), 
      _port(config.HttpsServerPort()), 
      _context(context) {};

  SslConfiguration(const SslConfiguration &other)
    : _enabled(other._enabled),
      _port(other._port),
      _context(other._context) {};

public:
  /** Returns true if SSL is enabled **/
  bool Enabled(void) const { return _enabled; }

  /** Returns the configured HTTPS SSL port **/
  port_t Port(void) const { return _port; }

  /** Returns the initialized SslContext, or else nullptr **/
  SslContext Context(void) const { return _context; }

private:
  bool _enabled;
  port_t _port;
  SslContext _context;
};


/**
 * Static helpers for initializing and terminating SSL
 */
class Ssl
{
public:
  /**
   * Returns an SslConfiguration, potentially with an intialized SslContext
   * if SSL is enabled
   */
  static SslConfiguration IntializeSsl(const Configuration &config);

  /**
   * Terminates an SslContext if SSL is enabled
   */
  static SslConfiguration TerminateSsl(const SslConfiguration &config);
};


/**
 * An AcceptSocket that accepts SSL connections and sends/receives data
 */
class SslAcceptSocket
  : public AcceptSocket
{
public:
  SslAcceptSocket(const AcceptSocket &socket, const SslConfiguration &config);

public:
  /** Encrypts and sends data to the underlying socket **/
  virtual size_t Send(const std::string &data);

  /** Reads and decryptes data from the underlying socket **/
  virtual size_t Recv(std::array<char, ACCEPT_BUFFER_SIZE>& buffer);

  /** Closes the underlying SSL connection and socket **/
  virtual void CloseSocket(void);

private:
  SslConnection _ssl;
};


/**
 * A ListenSocket that listens for SSL connections to accept
 */
class SslListenSocket
  : public ListenSocket
{
public:
  SslListenSocket(const SslConfiguration &config);

  /** Accepts an incoming SSL socket connection **/
  virtual std::unique_ptr<AcceptSocket> Accept(void);

private:
  SslConfiguration _config;
};

#endif // __SSL_H__