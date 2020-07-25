#include "IpAddress.h"
#include "Log.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>

IpAddress::IpAddress(const Socket &socket)
{
    _address = {};
    _text = "";

    sockaddr_in addr;
    socklen_t addr_size = sizeof(sockaddr_in);
    int result = getpeername(socket.FileDescriptor(), (sockaddr *)&addr, &addr_size);

    if (result == 0) {
      _address = addr;

      char buffer[INET6_ADDRSTRLEN];
      const char *pResult = inet_ntop(addr.sin_family, &(addr.sin_addr), buffer, INET6_ADDRSTRLEN);

      if (pResult != nullptr) {
        _text = pResult;
      } else {
        Log::LogSocketError("inet_ntop", socket.FileDescriptor(), 0);
        _text = "unknown";
      }
    } else {
      Log::LogSocketError("getpeername", socket.FileDescriptor(), result);
      _text = "unknown";
    }
}
