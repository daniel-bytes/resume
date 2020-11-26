#ifndef __SSLSOCKET_H__
#define __SSLSOCKET_H__

#include "AcceptSocket.h"
#include "ListenSocket.h"
#include "Typedefs.h"
#include "shared/Configuration.h"
#include <memory>
#include <string>

/**
 * Wrapper to an opaque pointer to an SSL context.
 * Defined so that OpenSSL implementation can remain in SslSocket.cpp.
 */
struct SslContext
{
  bool enabled;
  port_t port;
  void *context;
};


/**
 * Opaque pointer to an SSL connection.
 * Defined so that OpenSSL implementation can remain in SslSocket.cpp.
 */
typedef void* SslConnection;


/**
 * Static helpers for initializing and terminating SSL
 */
class Ssl
{
public:
  static SslContext IntializeSsl(const Configuration &config);
  static void TerminateSsl(const SslContext &context);
};


/**
 * An AcceptSocket accepts SSL connections and sends/receives data
 */
class SslAcceptSocket
  : public AcceptSocket
{
public:
  SslAcceptSocket(const AcceptSocket &socket, const SslContext &context);

public:
  virtual size_t Send(const std::string &data);
  virtual size_t Recv(std::array<char, ACCEPT_BUFFER_SIZE>& buffer);
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
  SslListenSocket(const SslContext &sslContext);
  virtual std::unique_ptr<AcceptSocket> Accept(void);

private:
  SslContext _sslContext;
};

#endif // __SSLSOCKET_H__