#ifndef UNORDERED_MAP_H_
#define UNORDERED_MAP_H_

#include "hashtable.h"

namespace my_map {
template <class K, class V>
class unordered_map {
  struct KeyOfValue {
    // val->key 映射
    const K& operator()(const std::pair<K, V>& val) { return val.first; }
  };

 public:
  typedef hashtable_bucket::HashTable<K, std::pair<K, V>, KeyOfValue> HT;
  typedef typename hashtable_bucket::HashTable<
      K, std::pair<K, V>, KeyOfValue,
      hashtable_bucket::hashFunction<K>>::iterator iterator;
  std::pair<iterator, bool> insert(const std::pair<K, V>& val) {
    return _tables.insert(val);
  }
  iterator begin() { return _tables.begin(); }
  iterator end() { return _tables.end(); }
  // 运算符重载
  V& operator[](const K& key) {
    // iterator res = _tables.find(key);
    // return res->second;
    std::pair<iterator, bool> res = _tables.insert(std::make_pair(key, V()));
    return res.first->second;
  }
  iterator find(const K& key) { return _tables.find(key); }
  iterator erase(const K& key) { return _tables.erase(key); }

 private:
  HT _tables;
};
}  // namespace my_map

#endif