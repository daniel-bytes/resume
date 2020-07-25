#ifndef __FILECACHE_H__
#define __FILECACHE_H__

#include <map>
#include <string>
#include <optional>

class FileCache
{
public:
  std::optional<std::string> Get(const std::string &path);

private:
  std::map<std::string, std::string> _cache;
};

#endif //__FILECACHE_H__
