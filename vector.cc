#include <iostream>
#include <vector>
using namespace std;

template <class T>
class myvector {
 private:
  T* _start;       // 数据头
  T* _end;         // 数据末尾
  T* _endofspace;  // 存储空间的末尾
 public:
  // 构造函数
  myvector() : _start(nullptr), _end(nullptr), _endofspace(nullptr) {}

  myvector(size_t n, const T& val = T())
      : _start(nullptr), _end(nullptr), _endofspace(nullptr) {
    reserve(n);  // 扩容
    while (n--) {
      push_back(val);
    }
  }
  myvector(const myvector& v)
      : _start(nullptr), _end(nullptr), _endofspace(nullptr) {
    // 扩容
    reserve(v.capacity());
    // 遍历拷贝数据
    // 问题：为啥必须使用 cbegin()
    const T* iter = v.cbegin();
    const T* end = v.cend();
    while (iter != end) {
      *_end = *iter;
      _end++;
      iter++;
    }
  }
  myvector(T* _begin, T* _last)
      : _start(nullptr), _end(nullptr), _endofspace(nullptr) {
    while (_begin != _last) {
      push_back(*_begin);
      _begin++;
    }
  }

  // .begin()
  // 返回指向第一个数据的指针
  T* begin() { return _start; }
  // .end()
  // 返回指向末尾数据的指针
  T* end() { return _end; }
  // 返回const 指针
  const T* cbegin() const { return _start; }

  const T* cend() const { return _end; }
  // .size()
  // 返回有效数据的大小
  size_t size() const { return _end - _start; }
  // .capacity()
  // 返回容量大小
  size_t capacity() const { return _endofspace - _start; }

  // 在末尾添加一个值
  void push_back(const T& val) {
    if (_end == _endofspace) {
      // 空间不足，需要二次分配
      size_t newspace = _endofspace == nullptr ? 1 : 2 * capacity();
      reserve(newspace);
    }
    *_end = val;
    _end++;
  }

  // 在pos前插入一个值
  T* insert(T* pos, const T& val) {
    if (_end == _endofspace) {
      // 需要扩容
      size_t sz = pos - _start;
      size_t newspace = capacity() == 0 ? 2 : capacity() * 2;
      reserve(newspace);
      pos = _start + sz;
    }
    T* last = _end - 1;
    // 向后移动数据
    while (pos <= last) {
      *(last + 1) = *last;
      last--;
    }
    // 插入数据
    *pos = val;
    _end++;
    return pos;
  }
  // 在pos前插入n个相同的值
  T* insert(T* pos, size_t n, const T& val) {
    // 在指定元素前插入n个val
    if (size() + n > capacity()) {
      // 需要扩容
      size_t sz = pos - _start;
      // size_t newspace = capacity() == 0 ? n :
      // capacity() * 2;
      size_t newspace = capacity();
      if (capacity() == 0) {
        newspace = n;
      } else {
        size_t cnt = (size() + n) / capacity();
        while (cnt--) {
          newspace *= 2;
        }
      }
      reserve(newspace);
      pos = _start + sz;
    }
    T* last = _end - 1;
    _end += n;
    // 向后移动数据
    while (pos <= last) {
      *(last + n) = *last;
      last--;
    }
    // 插入数据
    while (n--) {
      *pos = val;
      pos++;
    }
    //		_end+=n;
    return pos;
  }
  // 在pos前，插入另一个同类型myvector [left,right)区间内的值
  T* insert(T* pos, const T* left, const T* right) {
    // 指定元素前，插入另一个相同类型vector
    // [first, last) 区间内的元素
    //		cout<<*left<<" "<<*right<<endl;
    //		cout<<size()<<endl;
    size_t n = 0;
    while (left != right) {
      right--;
      n++;
    }
    if (size() + n > capacity()) {
      // 需要扩容
      size_t sz = pos - _start;
      //			size_t newspace = capacity() == 0 ? n :
      // capacity() * 2;
      size_t newspace = capacity();
      if (capacity() == 0) {
        newspace = n;
      } else {
        size_t cnt = (size() + n) / capacity();
        while (cnt--) {
          newspace *= 2;
        }
      }
      reserve(newspace);
      pos = _start + sz;
    }
    //		cout<<size()<<" "<<capacity()<<endl;
    T* last = _end - 1;
    _end += n;
    while (pos <= last) {
      *(last + n) = *last;
      last--;
    }
    // 插入数据
    while (n--) {
      *pos = *left;
      pos++;
      left++;
    }
    return pos;
  }

  // 删除最后一个值
  void pop_back() {
    // 删除最后一个元素
    if (_end != _start) _end--;
  }

  void clear() {
    // 清空所有元素
    if (_start == nullptr) {
      return;
    }
    delete[] _start;
    _start = _endofspace = _end = nullptr;
  }
  // 删除pos位置的值
  T* erase(T* pos) {
    // 删除指向的元素
    T* tmp = pos + 1;
    // pos后边的数据，依次向前
    while (tmp != _end) {
      *(tmp - 1) = *tmp;
      tmp++;
    }
    _end--;
    return pos;
  }
  // 删除[left,right)区间的值
  T* erase(T* left, T* right) {
    // 删除 [left, right) 中的元素
    //		T* tmp = right;
    while (right != _end) {
      *left = *right;
      left++;
      right++;
    }
    _end = left;
    return _end;
  }
  // 返回第一个元素值
  T& front() { return *_start; }
  // 返回最后一个元素的值
  T& back() { return *(_end - 1); }
  // 判断是否为空
  bool empty() { return _start == _end; }
  // 重新更改有效数据的大小
  void resize(size_t n, const T& val = T()) {
    cout << size() << " " << capacity() << endl;
    if (n > size()) {
      // n大于有效数据长度
      if (n > capacity()) {
        // 扩容
        reserve(n);
      }
      //			T* last = _start + n*sizeof(T);
      T* last = _start + n;
      // 多余部分使用默认值填充
      while (_end != last) {
        *_end = val;
        _end++;
      }
    } else {
      // 截断
      _end = _start + n * sizeof(T);
    }
  }
  // 更改容量的大小
  void reserve(size_t n) {
    // 扩容
    if (n > capacity()) {
      size_t sz = size();
      T* tmp = new T[n];
      for (size_t i = 0; i < sz; i++) {
        tmp[i] = _start[i];
      }
      // 删除旧空间
      delete[] _start;
      _start = tmp;
      _end = _start + sz;
      _endofspace = _start + n;
    }
  }
  // swap 交换两个myvector
  void swap(myvector& v) {
    // 交换各自的地址信息
    std::swap(_start, v._start);
    std::swap(_end, v._end);
    std::swap(_endofspace, v._endofspace);
  }
  // 重载 = 运算符
  myvector& operator=(myvector& v) {
    swap(v);  // myvector内部实现
    return *this;
  }
  // 重载 [] 运算符
  T& operator[](size_t pos) {
    if (pos < size()) {
      return _start[pos];
    } else {
      exit(1);
    }
  }
  // 析构函数
  ~myvector() {
    if (_start) {
      delete[] _start;
      _start = _end = _endofspace = nullptr;
    }
  }
};

int main() {
  // 构造方式测试
  myvector<int> nums(10, 3);
  cout << "nums: ";
  for (int i = 0; i < 10; i++) {
    cout << nums[i];
  }
  cout << endl;
  myvector<int> nums1(10);
  cout << "nums1: ";
  for (int n : nums1) {
    cout << n;
  }
  cout << endl;
  myvector<int> nums2(nums);
  cout << "nums2: ";
  for (int n : nums2) {
    cout << n;
  }
  cout << endl;
  myvector<int> nums3(nums.begin() + 2, nums.end());
  cout << "nums3: ";
  for (int n : nums3) {
    cout << n;
  }
  cout << endl;
  // push_back
  myvector<string> s;
  cout << "push_back: ";
  s.push_back("abcd");
  cout << s[0] << endl;
  cout << "insert: ";
  s.insert(s.begin(), "x");
  cout << s[0] << " " << s[1] << endl;
  cout << "erase: ";
  s.erase(s.begin());
  cout << s[0] << endl;

  // 其他
  myvector<int> numo;
  cout << "numo :";
  for (int i = 0; i < 10; i++) {
    numo.push_back(i);
    cout << i;
  }
  cout << endl;
  numo.erase(numo.begin() + 2, numo.begin() + 4);
  cout << "numo (erase): ";
  for (int n : numo) {
    cout << n;
  }
  cout << endl;
  numo.insert(numo.begin() + 2, 10, 5);
  cout << "numo (insert): ";
  for (int n : numo) {
    cout << n;
  }
  cout << endl;
  //	cout<<nums.size()<<endl;
  cout << "numo (insert): ";
  numo.insert(numo.begin() + 2, nums.begin() + 1, nums.begin() + 4);
  for (int n : numo) {
    cout << n;
  }
  cout << endl;
  cout << "numo (front, back) :";
  cout << numo.front() << " " << numo.back() << endl;
  cout << numo.empty() << endl;
  cout << "numo (size, capacity): ";
  cout << numo.size() << " " << numo.capacity() << endl;

  // resize
  numo.resize(25, 2);
  cout << "numo (resize): ";
  for (int n : numo) {
    cout << n;
  }
  cout << endl;
  cout << numo.size() << " " << numo.capacity() << endl;

  // 验证 swap 和 = 号运算符
  cout << "swap(nums1, nums2): " << endl;
  cout << "nums1: ";
  nums1.swap(nums2);
  for (int n : nums1) {
    cout << n;
  }
  cout << endl;
  cout << "nums2: ";
  for (int n : nums2) {
    cout << n;
  }
  cout << endl;
  // 测试 = 运算符
  cout << "nums4 = nums1: ";
  myvector<int> nums4 = nums1;
  for (int n : nums4) {
    cout << n;
  }
  cout << endl;

  return 0;
}
