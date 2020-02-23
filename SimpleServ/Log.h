#ifndef __LOG_H__
#define __LOG_H__

#include <iostream>
#include <iomanip>
#include <ostream>
#include <chrono>
#include <ctime>

namespace Log {
  inline std::ostream& log(const char *level) {
    auto t = time(nullptr);

    return std::cout 
      << "[" << level << "] [" 
      << std::put_time(std::gmtime(&t), "%FT%T%z") 
      << "] ";
  }

  inline std::ostream& info() {
    return log("INFO");
  }

  inline std::ostream& error() {
    return log("ERROR");
  }
}

#endif //__LOG_H__