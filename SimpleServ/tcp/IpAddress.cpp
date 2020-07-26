#include "IpAddress.h"
#include "Log.h"
#include <array>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define LOGGER "IpAddress"

IpAddress::IpAddress(const AcceptSocket &socket)
{
    _address = {};
    _text = "";

    sockaddr_in addr;
    socklen_t addr_size = sizeof(sockaddr_in);
    int result = getpeername(socket.FileDescriptor(), (sockaddr *)&addr, &addr_size);

    if (result == 0) {
      _address = addr;

      std::array<char, INET6_ADDRSTRLEN> buffer = {0};
      const char *pResult = inet_ntop(addr.sin_family, &(addr.sin_addr), buffer.begin(), buffer.size());

      if (pResult != nullptr) {
        _text = pResult;
      } else {
        Log::LogSocketError(LOGGER, "inet_ntop", socket.FileDescriptor(), 0);
        _text = "unknown";
      }
    } else {
      Log::LogSocketError(LOGGER, "getpeername", socket.FileDescriptor(), result);
      _text = "unknown";
    }
}
