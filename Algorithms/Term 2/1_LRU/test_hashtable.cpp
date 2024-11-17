#include "hashtable.hpp"
#include <cassert>
#include <iostream>
#include <string>

int main() {
  HashTable<std::string, int> table;

  // Тестирование вставки и доступа
  table.insert({"one", 1});
  table.insert({"two", 2});
  table["three"] = 3;

  assert(table.size() == 3);
  assert(table["one"] == 1);
  assert(table["two"] == 2);
  assert(table["three"] == 3);

  // Тестирование find и count
  auto it = table.find("two");
  assert(it != table.end());
  assert(it->value == 2);
  assert(table.count("two") == 1);
  assert(table.count("four") == 0);

  // Тестирование erase
  table.erase("two");
  assert(table.size() == 2);
  assert(table.count("two") == 0);

  // Тестирование итераторов
  int sum = 0;
  for (auto &node : table) {
    sum += node.value;
  }
  assert(sum == 4); // one(1) + three(3)

  // Тестирование clear
  table.clear();
  assert(table.empty());

  std::cout << "All HashTable tests passed!" << std::endl;
  return 0;
}
