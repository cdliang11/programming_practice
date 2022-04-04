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
      hashtable_bucket::HashFunction<K>>::iterator iterator;
  std::pair<iterator, bool> insert(const std::pair<K, V>& val) {
    return tables_.insert(val);
  }
  iterator begin() { return tables_.begin(); }
  iterator end() { return tables_.end(); }
  // 运算符重载
  V& operator[](const K& key) {
    // iterator res = tables_.find(key);
    // return res->second;
    std::pair<iterator, bool> res = tables_.insert(std::make_pair(key, V()));
    return res.first->second;
  }
  iterator find(const K& key) { return tables_.find(key); }
  iterator erase(const K& key) { return tables_.erase(key); }

 private:
  HT tables_;
};
}  // namespace my_map

#endif