#include <iostream>

using namespace std;

template <class T>
class MyVector {
 public:
  // 构造函数
  MyVector() : start_(nullptr), end_(nullptr), endofspace_(nullptr) {}

  MyVector(size_t n, const T& val = T())
      : start_(nullptr), end_(nullptr), endofspace_(nullptr) {
    reserve(n);  // 扩容
    while (n--) {
      push_back(val);
    }
  }
  MyVector(const MyVector& v)
      : start_(nullptr), end_(nullptr), endofspace_(nullptr) {
    // 扩容
    reserve(v.capacity());
    // 遍历拷贝数据
    // 问题：为啥必须使用 cbegin()
    const T* iter = v.cbegin();
    const T* end = v.cend();
    while (iter != end) {
      *end_ = *iter;
      end_++;
      iter++;
    }
  }
  MyVector(T* _begin, T* _last)
      : start_(nullptr), end_(nullptr), endofspace_(nullptr) {
    while (_begin != _last) {
      push_back(*_begin);
      _begin++;
    }
  }

  // .begin()
  // 返回指向第一个数据的指针
  T* begin() { return start_; }
  // .end()
  // 返回指向末尾数据的指针
  T* end() { return end_; }
  // 返回const 指针
  const T* cbegin() const { return start_; }

  const T* cend() const { return end_; }
  // .size()
  // 返回有效数据的大小
  size_t size() const { return end_ - start_; }
  // .capacity()
  // 返回容量大小
  size_t capacity() const { return endofspace_ - start_; }

  // 在末尾添加一个值
  void push_back(const T& val) {
    if (end_ == endofspace_) {
      // 空间不足，需要二次分配
      size_t newspace = endofspace_ == nullptr ? 1 : 2 * capacity();
      reserve(newspace);
    }
    *end_ = val;
    end_++;
  }

  // 在pos前插入一个值
  T* insert(T* pos, const T& val) {
    if (end_ == endofspace_) {
      // 需要扩容
      size_t sz = pos - start_;
      size_t newspace = capacity() == 0 ? 2 : capacity() * 2;
      reserve(newspace);
      pos = start_ + sz;
    }
    T* last = end_ - 1;
    // 向后移动数据
    while (pos <= last) {
      *(last + 1) = *last;
      last--;
    }
    // 插入数据
    *pos = val;
    end_++;
    return pos;
  }
  // 在pos前插入n个相同的值
  T* insert(T* pos, size_t n, const T& val) {
    // 在指定元素前插入n个val
    if (size() + n > capacity()) {
      // 需要扩容
      size_t sz = pos - start_;
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
      pos = start_ + sz;
    }
    T* last = end_ - 1;
    end_ += n;
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
    //		end_+=n;
    return pos;
  }
  // 在pos前，插入另一个同类型MyVector [left,right)区间内的值
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
      size_t sz = pos - start_;
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
      pos = start_ + sz;
    }
    // cout<<size()<<" "<<capacity()<<endl;
    T* last = end_ - 1;
    end_ += n;
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
    if (end_ != start_) end_--;
  }

  void clear() {
    // 清空所有元素
    if (start_ == nullptr) {
      return;
    }
    delete[] start_;
    start_ = endofspace_ = end_ = nullptr;
  }
  // 删除pos位置的值
  T* erase(T* pos) {
    // 删除指向的元素
    T* tmp = pos + 1;
    // pos后边的数据，依次向前
    while (tmp != end_) {
      *(tmp - 1) = *tmp;
      tmp++;
    }
    end_--;
    return pos;
  }
  // 删除[left,right)区间的值
  T* erase(T* left, T* right) {
    // 删除 [left, right) 中的元素
    //		T* tmp = right;
    while (right != end_) {
      *left = *right;
      left++;
      right++;
    }
    end_ = left;
    return end_;
  }
  // 返回第一个元素值
  T& front() { return *start_; }
  // 返回最后一个元素的值
  T& back() { return *(end_ - 1); }
  // 判断是否为空
  bool empty() { return start_ == end_; }
  // 重新更改有效数据的大小
  void resize(size_t n, const T& val = T()) {
    cout << size() << " " << capacity() << endl;
    if (n > size()) {
      // n大于有效数据长度
      if (n > capacity()) {
        // 扩容
        reserve(n);
      }
      // T* last = start_ + n*sizeof(T);
      T* last = start_ + n;
      // 多余部分使用默认值填充
      while (end_ != last) {
        *end_ = val;
        end_++;
      }
    } else {
      // 截断
      end_ = start_ + n * sizeof(T);
    }
  }
  // 更改容量的大小
  void reserve(size_t n) {
    // 扩容
    if (n > capacity()) {
      size_t sz = size();
      T* tmp = new T[n];
      for (size_t i = 0; i < sz; i++) {
        tmp[i] = start_[i];
      }
      // 删除旧空间
      delete[] start_;
      start_ = tmp;
      end_ = start_ + sz;
      endofspace_ = start_ + n;
    }
  }
  // swap 交换两个MyVector
  void swap(MyVector& v) {
    // 交换各自的地址信息
    std::swap(start_, v.start_);
    std::swap(end_, v.end_);
    std::swap(endofspace_, v.endofspace_);
  }
  // 重载 = 运算符
  MyVector& operator=(MyVector& v) {
    swap(v);  // MyVector内部实现
    return *this;
  }
  // 重载 [] 运算符
  T& operator[](size_t pos) {
    if (pos < size()) {
      return start_[pos];
    } else {
      exit(1);
    }
  }
  // 析构函数
  ~MyVector() {
    if (start_) {
      delete[] start_;
      start_ = end_ = endofspace_ = nullptr;
    }
  }

 private:
  T* start_;       // 数据头
  T* end_;         // 数据末尾
  T* endofspace_;  // 存储空间的末尾
};

int main() {
  // 构造方式测试
  MyVector<int> nums(10, 3);
  cout << "nums: ";
  for (int i = 0; i < 10; i++) {
    cout << nums[i];
  }
  cout << endl;
  MyVector<int> nums1(10);
  cout << "nums1: ";
  for (int n : nums1) {
    cout << n;
  }
  cout << endl;
  MyVector<int> nums2(nums);
  cout << "nums2: ";
  for (int n : nums2) {
    cout << n;
  }
  cout << endl;
  MyVector<int> nums3(nums.begin() + 2, nums.end());
  cout << "nums3: ";
  for (int n : nums3) {
    cout << n;
  }
  cout << endl;
  // push_back
  MyVector<string> s;
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
  MyVector<int> numo;
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
  MyVector<int> nums4 = nums1;
  for (int n : nums4) {
    cout << n;
  }
  cout << endl;

  return 0;
}
