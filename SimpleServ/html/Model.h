#ifndef __MODEL_H__
#define __MODEL_H__

#include <string>
#include <map>

class Model
{
public:
  typedef std::string Key;
  typedef std::string Value;
  typedef std::map<Key, Value> Data;

public:
  Model() {}
  Model(const Data &data): _data(data) {}

public:
  const Value& Get(const Key &key) const {
    return _data.at(key);
  }

  bool Has(const Key &key) const {
    return _data.count(key) > 0;
  }

  void Set(const Key &key, const Value &value) {
    _data[key] = value;
  }

private:
  Data _data;
};

#endif //__MODEL_H__