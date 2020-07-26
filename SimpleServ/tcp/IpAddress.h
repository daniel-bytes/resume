#ifndef __IPADDRESS_H__
#define __IPADDRESS_H__

#include "AcceptSocket.h"
#include <string>

class IpAddress
{
public:
	IpAddress(const AcceptSocket &socket);

public:
  const sockaddr_in& Address() const {
    return _address;
  }

  const std::string& Text() const {
    return _text;
  }

private:
  sockaddr_in _address;
  std::string _text;
};

#endif //__IPADDRESS_H__