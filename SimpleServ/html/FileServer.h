#ifndef __FILESERVER_H__
#define __FILESERVER_H__

#include <unordered_map>
#include <string>
#include <optional>

/**
 * A simple file server, loading a file from disk
 * and returning in text/string format
 */
class FileServer
{
public:
  virtual std::optional<std::string> Get(const std::string &path);
};

/**
 * A file server that maintains an in-memory cache of file contents
 */
class CachingFileServer 
  : public FileServer
{
public:
  virtual std::optional<std::string> Get(const std::string &path);

private:
  std::unordered_map<std::string, std::string> _cache;
};

#endif //__FILESERVER_H__
