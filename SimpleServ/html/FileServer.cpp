#include "FileServer.h"
#include "Logger.h"
#include <fstream>

#define LOGGER "FileServer"

using namespace Logger::NdJson;

std::optional<std::string> 
FileServer::Get(const std::string &path)
{
  Trace(LOGGER, "FileServer::Get");

  std::ifstream file(path);
  if (file.bad()) {
    return {};
  }

  std::string fileContents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

  if (fileContents.size() == 0) {
    return {};
  }

  return fileContents;
}

std::optional<std::string> 
CachingFileServer::Get(const std::string &path)
{
  Trace(LOGGER, "CachingFileServer::Get");

  auto result = _cache.find(path);
  if (result != _cache.end()) {
    return result->second;
  }

  auto fileContents = FileServer::Get(path);
  
  if (fileContents.has_value()) {
    _cache[path] = fileContents.value();
  }
  
  return fileContents;
}