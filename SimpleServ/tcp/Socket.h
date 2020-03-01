#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>

#include <string>

class Socket
{
public:
  Socket() 
    : _socket(-1) {}

  ~Socket() {
    CloseSocket();
  }

  int FileDescriptor() const {
    return _socket; 
  }

  bool IsActive() const {
    return _socket > -1;
  }

  size_t Send(const std::string &data);

protected:
  void SetReusable();
  void SetNonBlocking();
  void CloseSocket();

protected:
  int _socket;
  const int _on = 1;
};

#endif //__SOCKET_H__