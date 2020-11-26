#include "shared/Logger.h"
#include "shared/Configuration.h"
#include "shared/Utilities.h"

namespace Logger {
  namespace NdJson {
    void Log(
      const std::string &level, 
      const std::string &logger,
      const std::string &message,
      const Object& context,
      std::ostream &output
    ) {
      static auto version = std::getenv("APP_VERSION");
      auto t = time(nullptr);

      output
        << "{ \"timestamp\": \"" << std::put_time(std::gmtime(&t), "%FT%T%z") << "\""
        << ", \"level\": " << std::quoted(level)
        << ", \"logger\": " << std::quoted(logger)
        << ", \"version\": \"" << version << "\"";

      if (message.size()) {
        output << ", \"message\": " << std::quoted(message);
      }

      for (auto kvp : context) {
        output << ", " << std::quoted(kvp.first) << ": ";

        if (const auto sPtr (std::get_if<std::string>(&kvp.second)); sPtr) {
          output << std::quoted(*sPtr);
        } else if (const auto iPtr (std::get_if<int>(&kvp.second)); iPtr) {
          output << *iPtr;
        } else if (const auto fPtr (std::get_if<float>(&kvp.second)); fPtr) {
          output << *fPtr;
        } else if (const auto bPtr (std::get_if<bool>(&kvp.second)); bPtr) {
          output << *bPtr;
        } else {
          output << "null";
        }
      }

      output << " }" << std::endl;
    }
    
    void Trace(
      const std::string &logger,
      const std::string &message,
      const Object& context
    ) {
      if (Utilities::GetEnvVar("TRACE").value_or("") == Configuration::True()) {
        Logger::NdJson::Log("TRACE", logger, message, context, std::cout);
      }
    }
    
    void Info(
      const std::string &logger,
      const std::string &message,
      const Object& context
    ) {
      Logger::NdJson::Log("INFO", logger, message, context, std::cout);
    }
    
    void Error(
      const std::string &logger,
      const std::exception &error,
      const Object& context
    ) {
      Logger::NdJson::Log("ERROR", logger, error.what(), context, std::cerr);
    }

    void Error(
      const std::string &logger,
      const std::string &message,
      const Object& context
    ) {
      Logger::NdJson::Log("ERROR", logger, message, context, std::cerr);
    }
    
    void SocketError(
      const std::string &logger,
      const std::string &socketFn,
      socket_t fd,
      result_t result
    ) {
      std::string strerror = "";

      try {
        strerror = std::strerror(errno);
      } catch(const std::exception &exc) {
        strerror = std::string("strerror failed: ") + exc.what();
      }
        
      Logger::NdJson::Log(
          "ERROR", 
          logger, 
          "A socket error occurred", 
          {
            { "socket_function", socketFn },
            { "file_descriptor", fd },
            { "result", result },
            { "errno", errno },
            { "strerror", strerror }
          }, 
          std::cerr);
    }
  }
}