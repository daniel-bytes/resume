#include "Log.h"

namespace Log {
  static std::ofstream *devNull = nullptr;

  std::ostream& DevNull() {
    if (devNull == nullptr) {
      devNull = new std::ofstream;
      devNull->setstate(std::ios_base::badbit);
    }
    
    return *devNull;
  }
}