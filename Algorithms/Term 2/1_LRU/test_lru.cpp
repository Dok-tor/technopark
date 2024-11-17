#include "lru.hpp"
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

int main() {
  LRUCache<std::string, int> cache(3, 1000);

  // Тестирование вставки и доступа
  cache.Put("one", 1);
  cache.Put("two", 2);
  cache.Put("three", 3);

  assert(cache.size() == 3);
  assert(*cache.Get("one") == 1);
  assert(*cache.Get("two") == 2);
  assert(*cache.Get("three") == 3);

  // Тестирование LRU механизма
  cache.Put("four", 4); // Должен удалить самый старый элемент ("one")
  assert(cache.size() == 3);
  assert(cache.Get("one") == nullptr);
  assert(*cache.Get("four") == 4);

  // Тестирование обновления элемента
  cache.Put("two", 22);
  assert(*cache.Get("two") == 22);

  // Тестирование ограничения по размеру
  LRUCache<std::string, std::vector<int>> small_cache(
      3, sizeof(std::vector<int>) * 2);
  std::vector<int> data(10, 1);

  assert(small_cache.Put("data1", data) == true);
  assert(small_cache.Put("data2", data) == true);
  assert(small_cache.Put("data3", data) == false); // Не должно влезть

  assert(small_cache.size() == 2);

  std::cout << "All LRUCache tests passed!" << std::endl;
  return 0;
}
