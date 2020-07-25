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

namespace Log {
  std::ostream& DevNull();

  inline std::ostream& Log(const char *level) {
    auto t = time(nullptr);

    return std::cout 
      << "[" << level << "] [" 
      << std::put_time(std::gmtime(&t), "%FT%T%z") 
      << "] ";
  }

  #ifdef _DEBUG
  inline std::ostream& Debug() {
    return Log("DEBUG");
  }
  #else
  inline std::ostream& Debug() {
    return DevNull();
  }
  #endif
  
  inline std::ostream& Info() {
    return Log("INFO");
  }

  inline std::ostream& Error() {
    return Log("ERROR");
  }

  inline void LogSocketError(const char *socketFn, int fd, int result) {
    Log::Error() << socketFn << " failed for socket " << fd << ": "
      << "result = [" << result << "], "
      << "errno = [" << errno << "], " 
      << "error message = [" << std::strerror(errno) << "]" 
      << std::endl;
  }
}

#endif //__LOG_H__