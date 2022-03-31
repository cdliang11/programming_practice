
#include <iostream>

#include "hashtable.h"

using namespace std;

int main() {
  hashtable_close::HashTable<string, string, hashtable_close::hashFunction<string>> table;
  table.insert(std::make_pair("123", "一二三"));
  table.insert(std::make_pair("123", "四五六"));

  cout<<table.find("123")<<endl;
  cout<<table.erase("456")<<endl;
  cout<<table.insert(make_pair("456","四五六"))<<endl;

  hashtable_close::HashTable<int, int, hashtable_close::hashFunction<int>> table_1;
  table_1.insert(make_pair(12, 22));
  cout<<table_1.find(12)<<endl;
  return 0;
}
