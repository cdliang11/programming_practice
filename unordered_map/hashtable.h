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

namespace hashtable_bucket {
// 开散列
template <class V>
struct HashNode {
  V _val;
  HashNode<V>* _next;
  HashNode(const V& val = V()) : _val(val), _next(nullptr) {}
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

template <class K, class V, class KOfV, class Hash = hashFunction<K>>
class HashTable;

template <class K, class V, class KOfV, class Hash = hashFunction<K>>
struct HashIterator {
  // 定义迭代器
  typedef HashTable<K, V, KOfV, Hash> HT;
  typedef HashNode<V> Node;
  typedef HashIterator<K, V, KOfV, Hash> iterator;
  HT* _hptr;
  Node* _node;
  HashIterator(Node* node, HT* _hptr) : _node(node), _hptr(_hptr) {}
  V* operator*() { return _node->_val; }
  V* operator->() { return &_node->_val; }
  bool operator!=(const iterator& iter) { return iter._node != _node; }
  bool operator==(const iterator& iter) { return iter._node == _node; }
  // 前置++
  iterator operator++() {
    if (_node->_next) {
      _node = _node->_next;
    } else {
      // 这个桶结束，接着寻找下一个
      KOfV kov;
      size_t idx = kov(_node->_val) % _hptr->_tables.size();
      idx++;
      for (; idx < _hptr->_tables.size(); idx++) {
        if (_hptr->_tables[idx]) {
          _node = _hptr->_tables[idx];
          break;
        }
      }
      if (idx == _hptr->_tables.size()) {
        _node = nullptr;
      }
    }
    return *this;
  }
  // 后置++
  iterator operator++(int) {
    // 保存副本
    iterator tmp = *this;
    ++(*this);
    return tmp;
  }
};

template <class K, class V, class KOfV, class Hash>
class HashTable {
 public:
  // template <class K, class V, class KOfV, class Hash = hashFunction<K>>
  // struct HashIterator;
  typedef HashIterator<K, V, KOfV, Hash> iterator;
  typedef HashNode<V> Node;
  HashTable(int n = 10) : _tables(n), _n(0) {}

  //.begin()
  iterator begin() {
    for (size_t i = 0; i < _tables.size(); i++) {
      if (_tables[i]) {
        return iterator(_tables[i], this);
      }
    }
    return iterator(nullptr, this);
  }
  //.end()
  iterator end() { return iterator(nullptr, this); }
  //.find()
  iterator find(const K& key) {
    KOfV kov;
    Hash hfunc;
    for (size_t i = 0; i < _tables.size(); i++) {
      Node* cur = _tables[i];
      while (cur) {
        if (hfunc(kov(cur->_val)) == key) {
          return iterator(cur, this);
        }
        cur = cur->_next;
      }
    }
    return iterator(nullptr, this);
  }
  //.erase()
  iterator erase(const K& key) {
    KOfV kov;
    Hash hfunc;
    // 除留余数法
    int idx = key % _tables.size();
    Node* cur = _tables[idx];
    Node* prev = nullptr;
    if (hfunc(kov(cur->_val)) == key) {
      Node* next = cur->_next;
      _tables[idx] = cur->_next;
      delete cur;
      cur = nullptr;
      _n--;
      return iterator(next, this);
    }
    while (cur) {
      Node* next = cur->_next;
      prev = cur;
      if (hfunc(kov(cur->_val)) == key) {
        prev->_next = cur->_next;
        delete cur;
        _n--;
        return iterator(next, this);
      }
      cur = cur->_next;
    }
    return iterator(nullptr, this);
  }

  //.insert()
  std::pair<iterator, bool> insert(const V& val) {
    Hash hfunc;
    KOfV kov;
    // 负载因子到1的时候扩容
    if (_n == _tables.size()) {
      int newsize = _n == 0 ? 10 : _n * 2;
      std::vector<Node*> newtable(newsize);
      for (size_t i = 0; i < _tables.size(); i++) {
        Node* cur = _tables[i];
        // 拷贝到新表
        while (cur) {
          Node* next = cur->_next;
          int idx = kov(cur->_val) % newtable.size();
          cur->_next = newtable[idx];
          newtable[idx] = cur;
          cur = next;
        }
        _tables[i] = nullptr;
      }
      swap(_tables, newtable);
    }

    int idx = hfunc(kov(val)) % _tables.size();
    Node* cur = _tables[idx];
    while (cur) {
      if (kov(cur->_val) == kov(val)) {
        return std::make_pair(iterator(cur, this), false);  // 已经存在
      }
      cur = cur->_next;
    }
    cur = new Node(val);
    cur->_next = _tables[idx];
    _tables[idx] = cur;
    _n++;
    return std::make_pair(iterator(cur, this), true);
  }

 private:
  std::vector<Node*> _tables;
  size_t _n = 0;  // 有效数据的个数
};
}  // namespace hashtable_bucket

#endif
