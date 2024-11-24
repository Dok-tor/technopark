// test_hashtable.cpp
#include "hashtable.hpp"
#include <cassert>
#include <iostream>
#include <string>

int main() {
  std::cout << "Starting tests...\n";

  // Тест 1: Проверка конструктора и метода empty()
  HashTable<int, std::string> ht;
  assert(ht.empty());
  std::cout << "Test 1 passed: Constructor and empty() check.\n";

  // Тест 2: Проверка insert() и size()
  ht.insert({1, "one"});
  ht.insert({2, "two"});
  ht.insert({3, "three"});
  assert(ht.size() == 3);
  std::cout << "Test 2 passed: insert() and size() check.\n";

  // Тест 3: Проверка operator[] и обновление значения
  ht[2] = "two (updated)";
  assert(ht[2] == "two (updated)");
  std::cout << "Test 3 passed: operator[] and value update.\n";

  // Тест 4: Проверка find() и count()
  auto it = ht.find(3);
  assert(it != ht.end());
  assert(it->value == "three");
  assert(ht.count(3) == 1);
  std::cout << "Test 4 passed: find() and count() check.\n";

  // Тест 5: Проверка erase()
  ht.erase(2);
  assert(ht.size() == 2);
  assert(ht.count(2) == 0);
  std::cout << "Test 5 passed: erase() check.\n";

  // Тест 6: Проверка итераторов
  int key_sum = 0;
  for (auto iter = ht.begin(); iter != ht.end(); ++iter) {
    key_sum += iter->key;
  }
  assert(key_sum == 1 + 3); // Ключи 1 и 3
  std::cout << "Test 6 passed: iterator check.\n";

  // Тест 7: Проверка rehash() через заполнение таблицы
  for (int i = 4; i <= 20; ++i) {
    ht.insert({i, "number " + std::to_string(i)});
  }
  assert(ht.size() == 19); // У нас уже есть 1 и 3, добавили 17 элементов
  assert(ht.load_factor() <= ht.max_load_factor());
  std::cout << "Test 7 passed: rehash() and load factor check.\n";

  // Тест 8: Проверка reserve()
  ht.reserve(50);
  assert(ht.load_factor() <= ht.max_load_factor());
  std::cout << "Test 8 passed: reserve() check.\n";

  // Тест 9: Проверка max_load_factor()
  ht.max_load_factor(0.5f);
  assert(ht.max_load_factor() == 0.5f);
  std::cout << "Test 9 passed: max_load_factor() check.\n";

  // Тест 10: Проверка clear()
  ht.clear();
  assert(ht.empty());
  std::cout << "Test 10 passed: clear() check.\n";

  // Тест 11: Работа с пользовательским типом данных
  HashTable<std::string, int> ht_str;
  ht_str.insert({"apple", 1});
  ht_str.insert({"banana", 2});
  ht_str.insert({"orange", 3});
  assert(ht_str.size() == 3);
  assert(ht_str.count("banana") == 1);
  assert(ht_str.find("orange")->value == 3);
  std::cout << "Test 11 passed: custom key type check.\n";

  // Тест 12: Проверка коллизий и линейного пробирования
  HashTable<int, std::string> ht_collision(4);
  ht_collision.insert({1, "one"});
  ht_collision.insert({5, "five"}); // Потенциальная коллизия
  ht_collision.insert({9, "nine"});
  assert(ht_collision.size() == 3);
  assert(ht_collision.count(5) == 1);
  assert(ht_collision.find(5)->value == "five");
  std::cout << "Test 12 passed: collision and probing check.\n";

  // Тест 13: Удаление несуществующего элемента
  size_t erased = ht_collision.erase(100);
  assert(erased == 0);
  std::cout << "Test 13 passed: erase non-existent key check.\n";

  // Тест 14: Повторная вставка после удаления
  ht_collision.erase(5);
  ht_collision.insert({5, "five (new)"});
  assert(ht_collision.count(5) == 1);
  assert(ht_collision.find(5)->value == "five (new)");
  std::cout << "Test 14 passed: reinsertion after erase check.\n";

  // Тест 15: Работа с большим количеством элементов
  HashTable<int, int> ht_large;
  for (int i = 0; i < 1000; ++i) {
    ht_large.insert({i, i * i});
  }
  assert(ht_large.size() == 1000);
  bool data_correct = true;
  for (int i = 0; i < 1000; ++i) {
    if (ht_large.count(i) == 0 || ht_large.find(i)->value != i * i) {
      data_correct = false;
      break;
    }
  }
  assert(data_correct);
  std::cout << "Test 15 passed: large data set check.\n";

  // Все тесты пройдены
  std::cout << "All tests passed successfully!\n";

  return 0;
}
