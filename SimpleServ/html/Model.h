#ifndef __MODEL_H__
#define __MODEL_H__

#include <string>
#include <unordered_map>

class Model
{
public:
  typedef std::string Key;
  typedef std::string Value;
  typedef std::unordered_map<Key, Value> Data;

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

  void Remove(const Key &key) {
    _data.erase(key);
  }

private:
  Data _data;
};

#endif //__MODEL_H__