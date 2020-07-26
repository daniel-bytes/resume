#include "FileCache.h"
#include "Log.h"
#include <fstream>

#define LOGGER "FileCache"

std::optional<std::string> 
FileCache::Get(const std::string &path)
{
  auto result = _cache.find(path);
  if (result != _cache.end()) {
    Log::Debug(LOGGER) << "Found [" << path << "] in cache" << std::endl;
    return result->second;
  }

  std::ifstream file(path);
  if (file.bad()) {
    Log::Debug(LOGGER) << "File [" << path << "] is not valid" << std::endl;
    return {};
  }

  std::string fileContents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

  if (fileContents.size() == 0) {
    Log::Debug(LOGGER) << "File [" << path << "] is empty, skipping" << std::endl;
    return {};
  }

  _cache[path] = fileContents;

  Log::Debug(LOGGER) << "Caching file [" << path << "]" << std::endl;

  return fileContents;
}