#include "Ssl.h"
#include "TcpError.h"
#include "shared/Logger.h"
#include <openssl/ssl.h>
#include <openssl/err.h>

using namespace Logger::NdJson;

/**
 * Inline helper for extracting an OpenSSL SSL_CTX from SslConfiguration
 */
inline SSL_CTX* GetSslContext(const SslConfiguration &config) {
  if (!config.Enabled()) {
    throw TcpError("SSL not enabled");
  } else if (config.Context() == nullptr) {
    throw TcpError("SSL context was null");
  }

  return static_cast<SSL_CTX*>(config.Context());
}

/**
 * Inline helper for extracting an OpenSSL SSL from an opaque SslConnection
 */
inline SSL* GetSsl(SslConnection conn) {
  if (conn == nullptr) {
    throw TcpError("SSL not enabled, SslConnection was null");
  }

  return static_cast<SSL*>(conn);
}

SslConfiguration
Ssl::IntializeSsl(const Configuration &config)
{
  Trace("Ssl", "Ssl::IntializeSsl");
  if (!config.SslEnabled()) {
    return SslConfiguration();
  }

  OpenSSL_add_all_algorithms();
  SSL_load_error_strings();

  SSL_CTX *sslContext = SSL_CTX_new(TLS_method());
  
  if ( sslContext == nullptr ) {
    // TODO: Convert to error message in throw
    ERR_print_errors_fp(stderr);
    throw TcpError("SSL failed to initialize");
  }

  if ( SSL_CTX_set_min_proto_version(sslContext, TLS1_2_VERSION) <= 0 ) {
    // TODO: Convert to error message in throw
    ERR_print_errors_fp(stderr);
    throw TcpError("SSL failed to set minimum TLS version to 1.2");
  }

  if ( SSL_CTX_set_mode(sslContext, SSL_MODE_AUTO_RETRY) <= 0 ) {
    // TODO: Convert to error message in throw
    ERR_print_errors_fp(stderr);
    throw TcpError("SSL failed to set SSL_MODE_AUTO_RETRY");
  }

  if ( SSL_CTX_use_certificate_chain_file(sslContext, config.SslCertFilePath().c_str()) <= 0 ) {
    // TODO: Convert to error message in throw
    ERR_print_errors_fp(stderr);
    throw TcpError("SSL failed to load certificate file");
  }

  if ( SSL_CTX_use_PrivateKey_file(sslContext, config.SslKeyFilePath().c_str(), SSL_FILETYPE_PEM) <= 0 ) {
    // TODO: Convert to error message in throw
    ERR_print_errors_fp(stderr);
    throw TcpError("SSL failed to load private key file");
  }

  if ( !SSL_CTX_check_private_key(sslContext) ) {
    throw TcpError("SSL failed, private key does not match the public certificate");
  }

  return SslConfiguration(config, sslContext);
}

SslConfiguration
Ssl::TerminateSsl(const SslConfiguration &config)
{
  Trace("Ssl", "Ssl::TerminateSsl");
  auto context = GetSslContext(config);

  if (context != nullptr) {
    SSL_CTX_free(context);
  }

  return SslConfiguration();
}


SslAcceptSocket::SslAcceptSocket(const AcceptSocket &socket, const SslConfiguration &config)
  : AcceptSocket(socket)
{
  Trace("Ssl", "SslAcceptSocket::SslAcceptSocket");
  auto sslContext = GetSslContext(config);

  // TODO: error checking
  SSL *ssl = SSL_new(sslContext);
  SSL_set_fd(ssl, FileDescriptor());
  SSL_accept(ssl);

  _ssl = ssl;
}

size_t 
SslAcceptSocket::Send(const std::string &data)
{
  Trace("SslAcceptSocket", "SslAcceptSocket::Send", { { "fd", _socket } });
  auto ssl = GetSsl(_ssl);
  size_t result = SSL_write(ssl, data.c_str(), data.size());

  if (result < 0) {
    SocketError("SslAcceptSocket", "SSL_write", _socket, result);
  }

  return result;
}

size_t
SslAcceptSocket::Recv(std::array<char, ACCEPT_BUFFER_SIZE>& buffer)
{
  Trace("SslAcceptSocket", "SslAcceptSocket::Recv", { { "fd", _socket } });
  auto ssl = GetSsl(_ssl);

  result_t result = SSL_read(ssl, buffer.begin(), buffer.size());

  if (result > 0) {
    return result;
  }
  
  ERR_print_errors_fp(stderr);
  return 0;
}

void
SslAcceptSocket::CloseSocket(void)
{
  Trace("SslAcceptSocket", "SslAcceptSocket::CloseSocket", { { "fd", _socket } });
  auto ssl = GetSsl(_ssl);

  // TODO: error checking
  SSL_free(ssl);
  
  AcceptSocket::CloseSocket();
}

SslListenSocket::SslListenSocket(const SslConfiguration &config)
  : ListenSocket(config.Port()), _config(config)
{
  Trace("SslListenSocket", "SslListenSocket::ctor");
}

std::unique_ptr<AcceptSocket>
SslListenSocket::Accept(void)
{
  Trace("SslListenSocket", "SslListenSocket::Accept", { { "fd", _socket } });
  auto acceptSocket = ListenSocket::Accept();

  return std::unique_ptr<AcceptSocket>(
    new SslAcceptSocket(*acceptSocket, _config)
  );
}
