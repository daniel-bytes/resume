#ifndef __LOG_H__
#define __LOG_H__

#include <errno.h>
#include <unordered_map>
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

#include "tcp/Socket.h"

/**
 * Logging functions.
 * Trace() is only enabled when the TRACE environment variable is set.
 */
namespace Logger {
  /**
   * Logs a trace message if the TRACE=true environment variable is present
   */
  void Trace(
    const std::string &logger,
    const std::string &message
  );

  /**
   * Logging methods that use the newline-delimited JSON format
   */
  namespace NdJson {
    /** A JSON object property **/
    typedef std::string Property;

    /** A JSON value **/
    typedef std::variant<std::string, int, float, bool> Value;

    /** A JSON object **/
    typedef std::unordered_map<Property, Value> Object;

    /**
     * Logs a JSON message to the current logger (STDOUT or STDERR)
     */
    void Log(
      const std::string &level, 
      const std::string &logger,
      const std::string &message,
      const Object& context,
      std::ostream &output
    );

    /**
     * Logs a JSON trace message to STDOUT
     */
    void Trace(
      const std::string &logger,
      const std::string &message,
      const Object& context = {}
    );

    /**
     * Logs a JSON info message to STDOUT
     */
    void Info(
      const std::string &logger,
      const std::string &message,
      const Object& context = {}
    );

    /**
     * Logs a JSON exception to STDERR
     */
    void Error(
      const std::string &logger,
      const std::exception &error,
      const Object& context = {}
    );

    /**
     * Logs a JSON error message to STDERR
     */
    void Error(
      const std::string &logger,
      const std::string &message,
      const Object& context = {}
    );

    /**
     * Logs a Socket error to STDOUT
     */
    void SocketError(
      const std::string &logger,
      const std::string &socketFn,
      socket_t fd,
      result_t result
    );
  }
}

#endif //__LOG_H__