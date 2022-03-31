#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>
#include <vector>

// 哈希表，使用闭散列缓解冲突
namespace hashtable_close {
// 三状态：空，满，删除
// 标记当前元素的状态
enum Status { EMPTY, FULL, DELETE };

template <class K, class V>
struct HashData {
  std::pair<K, V> _kv;
  Status _status = EMPTY;
};

template <class K>
struct hashFunction {
  size_t operator()(const K& key) { return key; }
};

template <>
struct hashFunction<std::string> {
  size_t operator()(const std::string& key) {
    // https://www.cnblogs.com/-clq/archive/2012/05/31/2528153.html
    // BKDR Hash 字符串转哈希
    size_t hashVal = 0;
    for (size_t i = 0; i < key.size(); i++) {
      hashVal = hashVal * 131 + key[i];
    }
    return hashVal;
  }
};

template <class K, class V, class Hash = hashFunction<K>>
class HashTable {
 private:
  std::vector<HashData<K, V>> _tables;
  size_t _n = 0;  // 存储的数据个数
 public:
  // 查找
  HashData<K, V>* find(const K& key) {
    if (_tables.size() == 0) {
      return nullptr;  // 空表
    }
    Hash hashfunc;  // 键值转换函数
    size_t start = hashfunc(key) % _tables.size();
    size_t i = 0;
    size_t idx = start + i;
    while (_tables[idx]._status != EMPTY) {
      if (_tables[idx]._kv.first == key && _tables[idx]._status == FULL) {
        return &_tables[idx];
      } else {
        i++;
        idx = start + i * i;  // 二次探测
        idx %= _tables.size();
      }
    }
    return nullptr;
  }
  // 插入
  bool insert(const std::pair<K, V>& kv) {
    if (find(kv.first)) {
      // 已经存在相同key值
      return false;
    }
    // 扩容
    if (_tables.size() == 0 || _n * 10 / _tables.size() >= 7) {
      size_t newsize = _tables.size() == 0 ? 10 : _tables.size() * 2;
      HashTable<K, V> newHashTable;
      newHashTable._tables.resize(newsize);
      // 扩容后，将元素位置重排，避免堆积
      // 利用额外一个新表
      for (auto& t : _tables) {
        if (t._status == FULL) {
          newHashTable.insert(t._kv);
        }
      }
      _tables.swap(newHashTable._tables);
    }
    Hash hashfunc;
    size_t start = hashfunc(kv.first) % _tables.size();
    size_t i = 0;
    size_t idx = start + i;
    // 冲突后，继续查找下一个
    while (_tables[idx]._status == FULL) {
      i++;
      idx = start + i * i;
      idx %= _tables.size();
    }
    // 找到空位，插入
    _tables[idx]._kv = kv;
    _tables[idx]._status = FULL;
    _n++;

    return true;
  }
  // 删除
  bool erase(const K& key) {
    // 直接更改状态
    HashData<K, V>* tmp = find(key);
    if (tmp == nullptr) {
      return false;
    } else {
      tmp->_status = DELETE;
      _n--;
      return true;
    }
  }
};
}  // namespace hashtable_close

#endif
