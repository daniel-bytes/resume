#ifndef __LOG_H__
#define __LOG_H__

#include <errno.h>
#include <map>
#include <string>
#include <iomanip>
#include <variant>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <ostream>
#include <chrono>
#include <ctime>
#include <cstring>

namespace Logger {
  void Trace(
    const std::string &logger,
    const std::string &message
  );

  namespace NdJson {
    typedef std::string Property;
    typedef std::variant<std::string, int, float, bool> Value;
    typedef std::map<Property, Value> Object;

    void Log(
      const std::string &level, 
      const std::string &logger,
      const std::string &message,
      const Object& context
    );

    void Trace(
      const std::string &logger,
      const std::string &message
    );

    void Info(
      const std::string &logger,
      const std::string &message,
      const Object& context
    );

    void Error(
      const std::string &logger,
      const std::runtime_error &error,
      const Object& context
    );

    void SocketError(
      const std::string &logger,
      const std::string &socketFn,
      int fd,
      int result
    );
  }
}

#endif //__LOG_H__