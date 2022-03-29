# include<iostream>

using namespace std;

template<class T>
class myvector{
private:
	T* _start; // ����ͷ
	T* _end; // ����ĩβ
	T* _endofspace; // �洢�ռ��ĩβ 
public:
	// ���캯��
	myvector(): _start(nullptr), _end(nullptr), _endofspace(nullptr) {}
	
	myvector(size_t n, const T& val=T()): _start(nullptr), _end(nullptr), _endofspace(nullptr){
		reserve(n); // ����
		while (n--){
			push_back(val);
		} 
	}
	myvector(const myvector& v): _start(nullptr), _end(nullptr), _endofspace(nullptr){
		// ���� 
		reserve(v.capacity());
		// ������������
		// ���⣺Ϊɶ����ʹ�� cbegin() 
		const T* iter = v.cbegin();
		const T* end = v.cend();
		while(iter != end){
			*_end = *iter;
			_end++;
			iter++; 
		}
	}
	myvector(T* _begin, T* _last): _start(nullptr), _end(nullptr), _endofspace(nullptr){
		while (_begin != _last){
			push_back(*_begin);
			_begin++;
		}
	} 
	
	
	// .begin()
	// ����ָ���һ�����ݵ�ָ��
	T* begin(){
		return _start;
	}
	// .end()
	// ����ָ��ĩβ���ݵ�ָ�� 
	T* end(){
		return _end;
	}
	// ����const ָ�� 
	const T* cbegin() const{
		return _start;
	}

	const T* cend() const{
		return _end;
	}
	// .size()
	// ������Ч���ݵĴ�С
	size_t size() const {
		return _end - _start;
	} 
	// .capacity()
	// ����������С 
	size_t capacity() const {
		return _endofspace - _start;
	}
	

	// ��ĩβ���һ��ֵ 
	void push_back(const T& val){
		if (_end == _endofspace){
			// �ռ䲻�㣬��Ҫ���η���
			size_t newspace = _endofspace == nullptr ? 1 : 2 * capacity();
			reserve(newspace);
		}
		*_end = val;
		_end++;
	}
	
	// ��posǰ����һ��ֵ 
	T* insert(T* pos, const T& val){
		if (_end == _endofspace){
			// ��Ҫ����
			size_t sz = pos - _start;
			size_t newspace = capacity() == 0 ? 2 : capacity() * 2;
			reserve(newspace);
			pos = _start + sz; 
		}
		T* last = _end - 1;
		// ����ƶ����� 
		while (pos <= last){
			*(last+1) = *last;
			last--;
		}
		// ��������
		*pos = val;
		_end++;
		return pos;
	}
	// ��posǰ����n����ͬ��ֵ 
	T* insert(T* pos, size_t n, const T& val){
		// ��ָ��Ԫ��ǰ����n��val
		if (size()+n > capacity()){
			// ��Ҫ����
			size_t sz = pos - _start;
//			size_t newspace = capacity() == 0 ? n : capacity() * 2;
			size_t newspace = capacity();
			if (capacity() == 0){
				newspace = n;
			}else{
				size_t cnt = (size()+n) / capacity();
				while(cnt--){
					newspace *= 2;
				}
			}
			reserve(newspace);
			pos = _start + sz;
		}
		T* last = _end - 1;
		_end += n;
		// ����ƶ����� 
		while (pos <= last){
			*(last+n) = *last;
			last--;
		}
		// ��������
		while (n--){
			*pos = val;
			pos++;
		} 
//		_end+=n;
		return pos;
		
	}
	// ��posǰ��������һ��ͬ����myvector [left,right)�����ڵ�ֵ 
	T* insert(T* pos, const T* left, const T* right){
		// ָ��Ԫ��ǰ��������һ����ͬ����vector
		// [first, last) �����ڵ�Ԫ��
//		cout<<*left<<" "<<*right<<endl;
//		cout<<size()<<endl;
		size_t n = 0;
		while(left != right){
			right--;
			n++;
		}
		if (size()+n > capacity()){
			// ��Ҫ����
			size_t sz = pos - _start;
//			size_t newspace = capacity() == 0 ? n : capacity() * 2;
			size_t newspace = capacity();
			if (capacity() == 0){
				newspace = n;
			}else{
				size_t cnt = (size()+n) / capacity();
				while(cnt--){
					newspace *= 2;
				}
			}
			reserve(newspace);
			pos = _start + sz;
		}
//		cout<<size()<<" "<<capacity()<<endl;
		T* last = _end - 1;
		_end += n;
		while (pos <= last){
			*(last+n) = *last;
			last--;
		}
		// ��������
		while(n--){
			*pos = *left;
			pos++;
			left++;
		} 
		return pos;
	}
	
	// ɾ�����һ��ֵ 
	void pop_back() {
		// ɾ�����һ��Ԫ��
		if (_end != _start)
			_end--; 
	}
	
	void clear(){
		// �������Ԫ�� 
		if (_start == nullptr){
			return;
		}
		delete [] _start;
		_start = _endofspace = _end = nullptr;
	}
	// ɾ��posλ�õ�ֵ 
	T* erase(T* pos){
		// ɾ��ָ���Ԫ��
		T* tmp = pos + 1;
		// pos��ߵ����ݣ�������ǰ
		while (tmp != _end){
			*(tmp - 1) = *tmp;
			tmp++;
		}
		_end--;
		return pos;
	}
	// ɾ��[left,right)�����ֵ 
	T* erase(T* left, T* right){
		// ɾ�� [left, right) �е�Ԫ��
//		T* tmp = right;
		while (right != _end){
			*left = *right;
			left++;
			right++;
		}
		_end = left;
		return _end;
	}
	// ���ص�һ��Ԫ��ֵ 
	T& front(){
		return *_start;
	}
	// �������һ��Ԫ�ص�ֵ 
	T& back(){
		return *(_end - 1);
	}
	// �ж��Ƿ�Ϊ�� 
	bool empty() {
		return _start == _end;
	} 
	// ���¸�����Ч���ݵĴ�С 
	void resize(size_t n, const T& val=T()){
		cout<<size()<<" "<<capacity()<<endl;
		if (n > size()){
			// n������Ч���ݳ���
			if (n > capacity()){
				// ����
				reserve(n); 
			}
//			T* last = _start + n*sizeof(T);
			T* last = _start + n;
			// ���ಿ��ʹ��Ĭ��ֵ��� 
			while (_end != last){
				*_end = val;
				_end++;
			}
		} else {
			// �ض� 
			_end = _start + n*sizeof(T);
		}
	}
	// ���������Ĵ�С 
	void reserve(size_t n){
		// ����
		if (n > capacity()){
			size_t sz = size();
			T* tmp = new T[n];
			for (size_t i = 0; i < sz; i++){
				tmp[i] = _start[i];
			}
			// ɾ���ɿռ�
			delete [] _start;
			_start = tmp;
			_end = _start + sz;
			_endofspace = _start + n; 
		} 
	}
	// ���� [] ����� 
	T& operator[] (size_t pos){
		if (pos < size()){
			return _start[pos];
		} else {
			exit(1);
		}
	}
	// �������� 
	~myvector(){
		if (_start){
			delete [] _start;
			_start = _end = _endofspace = nullptr;
		}
	}
};

int main(){
	// ���췽ʽ���� 
	myvector<int> nums(10, 3);
	for (int i = 0; i<10; i++){
		cout<<nums[i];
	}
	cout<<endl;
	myvector<int> nums1(10);
	for (int n : nums1){
		cout<<n;
	}
	cout<<endl;
	myvector<int> nums2(nums);
	for (int n : nums2){
		cout<<n;
	}
	cout<<endl;
	myvector<int> nums3(nums.begin()+2, nums.end());
	for (int n : nums3){
		cout<<n;
	}
	cout<<endl;
	// push_back
	myvector<string> s;
	s.push_back("abcd");
	cout<<s[0]<<endl;
	s.insert(s.begin(), "x");
	cout<<s[0]<<" "<<s[1]<<endl;
	s.erase(s.begin());
	cout<<s[0]<<endl;
	
	// ����
	myvector<int> numo;
	for (int i=0; i<10; i++){
		numo.push_back(i);
		cout<<i;
	}
	cout<<endl;
	numo.erase(numo.begin()+2, numo.begin()+4);
	for (int n : numo){
		cout<<n;
	}
	cout<<endl;
	numo.insert(numo.begin()+2, 10, 5);
	for (int n : numo){
		cout<<n;
	}
	cout<<endl;
//	cout<<nums.size()<<endl;
	numo.insert(numo.begin()+2, nums.begin()+1, nums.begin()+4);
	for (int n : numo){
		cout<<n;
	}
	cout<<endl;
	cout<<numo.front()<<" "<<numo.back()<<endl;
	cout<<numo.empty()<<endl;
	// capacity ������ 
	cout<<numo.size()<<" "<<numo.capacity()<<endl;
	
	//resize
	numo.resize(25, 2);
	for (int n : numo){
		cout<<n;
	}
	cout<<endl;
	cout<<numo.size()<<" "<<numo.capacity()<<endl;
	
	return 0;
}


