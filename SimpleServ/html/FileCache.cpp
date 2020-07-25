#include "FileCache.h"
#include <fstream>

std::optional<std::string> 
FileCache::Get(const std::string &path)
{
  auto result = _cache.find(path);
  if (result != _cache.end()) {
    return result->second;
  }

  std::ifstream file(path);
  if (file.bad()) {
    return {};
  }

  std::string fileContents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
  _cache[path] = fileContents;

  return fileContents;
}