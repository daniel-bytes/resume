#ifndef __MODEL_H__
#define __MODEL_H__

#include <string>
#include <unordered_map>

/**
 * A simple key/value pair data model for a dynamic HTML page
 */
class Model
{
public:
  typedef std::string Key;
  typedef std::string Value;
  typedef std::unordered_map<Key, Value> Data;

public:
  Model() {}
  Model(const Data &data): _data(data) {}
  Model(const Model &rhs): _data(rhs._data) {}

public:
  /** Returns the model data by key **/
  const Value& Get(const Key &key) const { return _data.at(key); }

  /** Returns true if the model has the key specified **/
  bool Has(const Key &key) const { return _data.count(key) > 0; }

  /** Sets data by key in the model **/
  void Set(const Key &key, const Value &value) { _data[key] = value; }

  /** Removes data from the model **/
  void Remove(const Key &key) {_data.erase(key); }

private:
  Data _data;
};

#endif //__MODEL_H__