#include "Logger.h"

namespace Logger {
  namespace NdJson {
    static bool traceEnabled = std::getenv("TRACE");

    void Log(
      const std::string &level, 
      const std::string &logger,
      const std::string &message,
      const Object& context
    ) {
      static auto version = std::getenv("APP_VERSION");
      auto t = time(nullptr);

      std::cout
        << "{ \"timestamp\": \"" << std::put_time(std::gmtime(&t), "%FT%T%z") << "\""
        << ", \"level\": " << std::quoted(level)
        << ", \"logger\": " << std::quoted(logger)
        << ", \"version\": \"" << version << "\"";

      if (message.size()) {
        std::cout << ", \"message\": " << std::quoted(message);
      }

      for (auto kvp : context) {
        std::cout << ", " << std::quoted(kvp.first) << ": ";

        if (const auto sPtr (std::get_if<std::string>(&kvp.second)); sPtr) {
          std::cout << std::quoted(*sPtr);
        } else if (const auto iPtr (std::get_if<int>(&kvp.second)); iPtr) {
          std::cout << *iPtr;
        } else if (const auto fPtr (std::get_if<float>(&kvp.second)); fPtr) {
          std::cout << *fPtr;
        } else if (const auto bPtr (std::get_if<bool>(&kvp.second)); bPtr) {
          std::cout << *bPtr;
        } else {
          std::cout << "null";
        }
      }

      std::cout << " }" << std::endl;
    }
    
    void Trace(
      const std::string &logger,
      const std::string &message
    ) {
      if (traceEnabled) {
        Logger::NdJson::Log("TRACE", logger, message, {});
      }
    }
    
    void Info(
      const std::string &logger,
      const std::string &message,
      const Object& context
    ) {
      Logger::NdJson::Log("INFO", logger, message, context);
    }
    
    void Error(
      const std::string &logger,
      const std::runtime_error &error,
      const Object& context
    ) {
      Logger::NdJson::Log("ERROR", logger, error.what(), context);
    }
    
    void SocketError(
      const std::string &logger,
      const std::string &socketFn,
      int fd,
      int result
    ) {
      Logger::NdJson::Log("ERROR", logger, "A socket error occurred", {
        { "socket_function", socketFn },
        { "file_descriptor", fd },
        { "result", result },
        { "errno", errno },
        { "strerror", std::strerror(errno) }
      });
    }
  }
}