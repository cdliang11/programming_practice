
#include "unordered_map.h"

using namespace my_map;

int main() {
  unordered_map<int, int> mp;
  mp.insert(std::make_pair(1, 1));
  mp[2] = 2;
  mp[3] = 3;
  mp[10] = 10;
  if (mp.find(10) != mp.end()) {
    std::cout << "find 10" << std::endl;
  }
  mp.erase(10);
  if (mp.find(10) == mp.end()) {
    std::cout << "erase 10" << std::endl;
  }

  return 0;
}