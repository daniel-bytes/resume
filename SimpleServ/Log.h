#ifndef __LOG_H__
#define __LOG_H__

#include <errno.h>

#include <iostream>
#include <iomanip>
#include <ostream>
#include <chrono>
#include <ctime>

namespace Log {
  inline std::ostream& Log(const char *level) {
    auto t = time(nullptr);

    return std::cout 
      << "[" << level << "] [" 
      << std::put_time(std::gmtime(&t), "%FT%T%z") 
      << "] ";
  }

  inline std::ostream& Debug() {
    return Log("DEBUG");
  }
  
  inline std::ostream& Info() {
    return Log("INFO");
  }

  inline std::ostream& Error() {
    return Log("ERROR");
  }

  inline void LogSocketError(const char *socketFn, int fd, int result) {
    Log::Error() << socketFn << " failed for socket " << fd << ": "
      << "result = [" << result << "], errno = [" << errno << "]" 
      << std::endl;
  }
}

#endif //__LOG_H__