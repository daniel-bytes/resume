#ifndef __LOG_H__
#define __LOG_H__

#include <errno.h>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <ostream>
#include <chrono>
#include <ctime>
#include <cstring>

//#define _DEBUG true

namespace Log {
  std::ostream& DevNull();

  inline std::ostream& Log(const char *level, const char *logger) {
    auto t = time(nullptr);

    return std::cout 
      << "[" << level << "] "
      << "[" << std::put_time(std::gmtime(&t), "%FT%T%z") << "] "
      << "[" << logger << "] ";
  }

  #ifdef _DEBUG
  inline std::ostream& Debug(const char *logger) {
    return Log("DEBUG", logger);
  }
  #else
  inline std::ostream& Debug(const char *logger) {
    return DevNull();
  }
  #endif
  
  inline std::ostream& Info(const char *logger) {
    return Log("INFO", logger);
  }

  inline std::ostream& Error(const char *logger) {
    return Log("ERROR", logger);
  }

  inline void LogSocketError(const char *logger, const char *socketFn, int fd, int result) {
    Log::Error(logger) << socketFn << " failed for socket " << fd << ": "
      << "result = [" << result << "], "
      << "errno = [" << errno << "], " 
      << "error message = [" << std::strerror(errno) << "]" 
      << std::endl;
  }

  inline void LogException(const char *logger, const std::string &ipAddress, const std::runtime_error &err) {
    Log::Error(logger) << "[" << ipAddress << "] " << err.what() << std::endl;
  }

  inline void LogException(const char *logger, const std::string &ipAddress, const std::string &requestId, const std::runtime_error &err) {
    Log::Error(logger) << "[" << requestId << "] " << "[" << ipAddress << "] " << err.what() << std::endl;
  }
}

#endif //__LOG_H__