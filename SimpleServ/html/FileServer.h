#ifndef __FILESERVER_H__
#define __FILESERVER_H__

#include <unordered_map>
#include <string>
#include <optional>

class FileServer
{
public:
  virtual std::optional<std::string> Get(const std::string &path);
};

class CachingFileServer 
  : public FileServer
{
public:
  virtual std::optional<std::string> Get(const std::string &path);

private:
  std::unordered_map<std::string, std::string> _cache;
};

#endif //__FILESERVER_H__
