#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>
#include <vector>

namespace hashtable_bucket {
// 开散列
template <class V>
struct HashNode {
  V _val;
  HashNode<V>* _next;
  HashNode(const V& val = V()) : _val(val), _next(nullptr) {}
};

template <class K>
struct HashFunction {
  size_t operator()(const K& key) { return key; }
};

template <>
struct HashFunction<std::string> {
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

template <class K, class V, class KOfV, class Hash = HashFunction<K>>
class HashTable;

template <class K, class V, class KOfV, class Hash = HashFunction<K>>
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
      size_t idx = kov(_node->_val) % _hptr->tables_.size();
      idx++;
      for (; idx < _hptr->tables_.size(); idx++) {
        if (_hptr->tables_[idx]) {
          _node = _hptr->tables_[idx];
          break;
        }
      }
      if (idx == _hptr->tables_.size()) {
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
  // template <class K, class V, class KOfV, class Hash = HashFunction<K>>
  // struct HashIterator;
  typedef HashIterator<K, V, KOfV, Hash> iterator;
  typedef HashNode<V> Node;
  HashTable(int n = 10) : tables_(n), n_(0) {}

  //.begin()
  iterator begin() {
    for (size_t i = 0; i < tables_.size(); i++) {
      if (tables_[i]) {
        return iterator(tables_[i], this);
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
    for (size_t i = 0; i < tables_.size(); i++) {
      Node* cur = tables_[i];
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
    int idx = key % tables_.size();
    Node* cur = tables_[idx];
    Node* prev = nullptr;
    if (hfunc(kov(cur->_val)) == key) {
      Node* next = cur->_next;
      tables_[idx] = cur->_next;
      delete cur;
      cur = nullptr;
      n_--;
      return iterator(next, this);
    }
    while (cur) {
      Node* next = cur->_next;
      prev = cur;
      if (hfunc(kov(cur->_val)) == key) {
        prev->_next = cur->_next;
        delete cur;
        n_--;
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
    if (n_ == tables_.size()) {
      int newsize = n_ == 0 ? 10 : n_ * 2;
      std::vector<Node*> newtable(newsize);
      for (size_t i = 0; i < tables_.size(); i++) {
        Node* cur = tables_[i];
        // 拷贝到新表
        while (cur) {
          Node* next = cur->_next;
          int idx = kov(cur->_val) % newtable.size();
          cur->_next = newtable[idx];
          newtable[idx] = cur;
          cur = next;
        }
        tables_[i] = nullptr;
      }
      swap(tables_, newtable);
    }

    int idx = hfunc(kov(val)) % tables_.size();
    Node* cur = tables_[idx];
    while (cur) {
      if (kov(cur->_val) == kov(val)) {
        return std::make_pair(iterator(cur, this), false);  // 已经存在
      }
      cur = cur->_next;
    }
    cur = new Node(val);
    cur->_next = tables_[idx];
    tables_[idx] = cur;
    n_++;
    return std::make_pair(iterator(cur, this), true);
  }

 private:
  std::vector<Node*> tables_;
  size_t n_ = 0;  // 有效数据的个数
};
}  // namespace hashtable_bucket

// 哈希表，使用闭散列缓解冲突
// TODO: 加入迭代器
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
struct HashFunction {
  size_t operator()(const K& key) { return key; }
};

template <>
struct HashFunction<std::string> {
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

template <class K, class V, class Hash = HashFunction<K>>
class HashTable {
 public:
  // 查找
  HashData<K, V>* find(const K& key) {
    if (tables_.size() == 0) {
      return nullptr;  // 空表
    }
    Hash hashfunc;  // 键值转换函数
    size_t start = hashfunc(key) % tables_.size();
    size_t i = 0;
    size_t idx = start + i;
    while (tables_[idx]._status != EMPTY) {
      if (tables_[idx]._kv.first == key && tables_[idx]._status == FULL) {
        return &tables_[idx];
      } else {
        i++;
        idx = start + i * i;  // 二次探测
        idx %= tables_.size();
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
    if (tables_.size() == 0 || n_ * 10 / tables_.size() >= 7) {
      size_t newsize = tables_.size() == 0 ? 10 : tables_.size() * 2;
      HashTable<K, V> newHashTable;
      newHashTable.tables_.resize(newsize);
      // 扩容后，将元素位置重排，避免堆积
      // 利用额外一个新表
      for (auto& t : tables_) {
        if (t._status == FULL) {
          newHashTable.insert(t._kv);
        }
      }
      tables_.swap(newHashTable.tables_);
    }
    Hash hashfunc;
    size_t start = hashfunc(kv.first) % tables_.size();
    size_t i = 0;
    size_t idx = start + i;
    // 冲突后，继续查找下一个
    while (tables_[idx]._status == FULL) {
      i++;
      idx = start + i * i;
      idx %= tables_.size();
    }
    // 找到空位，插入
    tables_[idx]._kv = kv;
    tables_[idx]._status = FULL;
    n_++;

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
      n_--;
      return true;
    }
  }

 private:
  std::vector<HashData<K, V>> tables_;
  size_t n_ = 0;  // 存储的数据个数
};
}  // namespace hashtable_close

#endif
