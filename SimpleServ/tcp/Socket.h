#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <stdexcept>
#include <string>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>

#include "Typedefs.h"
#include "TcpError.h"

/**
 * An error that occurs when a Socket is used before being initialized.
 * T
 */
class InvalidSocketError 
  : public std::runtime_error
{
public:
  InvalidSocketError()
    : std::runtime_error("Socket was not initialized") {}
};

/**
 * A wrapper around a socket file descriptor.
 * Code that owns a Socket is expected to call CloseSocket() when
 * no longer used (no destructor RAII is used).
 * Care must be taken if multiple copies of the Socket exist to not use
 * the copy after CloseSocket() is called.
 */
class Socket
{
public:
  Socket(socket_t fd = -1) 
    : _socket(fd) {}

  Socket(const Socket &rhs) 
    : _socket(rhs._socket) {}

  socket_t FileDescriptor() const {
    return _socket; 
  }

  bool IsActive() const {
    return _socket > -1;
  }

  size_t Send(const std::string &data);

  TcpError GetSocketError();

  void CloseSocket();

protected:
  void SetReusable();
  void SetNonBlocking();
  void AssertValid() { if (!IsActive()) throw InvalidSocketError(); }

protected:
  socket_t _socket;
};

#endif //__SOCKET_H__