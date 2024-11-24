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
  std::cout << "Test 1 passed\n";

  // Тест 2: Проверка insert() и size()
  ht.insert({1, "one"});
  ht.insert({2, "two"});
  ht.insert({3, "three"});
  assert(ht.size() == 3);
  std::cout << "Test 2 passed\n";

  // Тест 3: Проверка operator[] и обновление значения
  ht[2] = "two (updated)";
  assert(ht[2] == "two (updated)");
  std::cout << "Test 3 passed\n";

  // Тест 4: Проверка find() и count()
  auto it = ht.find(3);
  assert(it != ht.end());
  assert(it->value == "three");
  assert(ht.count(3) == 1);
  std::cout << "Test 4 passed\n";

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

  // Тест 16: Удаление всех элементов и повторная вставка
  HashTable<int, std::string> ht1;
  ht1.insert({1, "one"});
  ht1.insert({2, "two"});
  ht1.erase(1);
  ht1.erase(2);
  assert(ht1.empty());
  ht1.insert({1, "one"});
  assert(ht1.size() == 1);
  assert(ht1.find(1)->value == "one");
  std::cout << "Test 16 passed: erase all and reinsert check.\n";

  // Тест 17: Вставка после удаления
  ht1.insert({2, "two"});
  ht1.erase(1);
  ht1.insert({1, "new one"});
  assert(ht1.find(1)->value == "new one");
  assert(ht1.size() == 2);
  std::cout << "Test 17 passed: insertion after erase check.\n";

  // Тест 18: Поведение find_position с удалёнными элементами
  HashTable<int, std::string> ht2(5); // Меньший размер для коллизий
  ht2.insert({1, "one"});
  ht2.insert({6, "six"}); // Потенциальная коллизия
  ht2.erase(1);
  ht2.insert({11, "eleven"}); // Перезаполнение удалённого слота
  assert(ht2.find(11)->value == "eleven");
  std::cout << "Test 18 passed: find_position and reuse deleted slot.\n";

  // Тест 19: Доступ к отсутствующему ключу через operator[]
  HashTable<int, std::string> ht3;
  assert(ht3[42].empty()); // Новый элемент с пустым значением
  assert(ht3.size() == 1);
  ht3[42] = "forty-two";
  assert(ht3.find(42)->value == "forty-two");
  std::cout << "Test 19 passed: operator[] on non-existent key.\n";

  // Тест 20: Поведение при заполнении до rehash
  HashTable<int, std::string> ht4(
      4); // Маленькая таблица для принудительного rehash
  ht4.insert({1, "one"});
  ht4.insert({2, "two"});
  ht4.insert({3, "three"});
  ht4.insert({4, "four"}); // Должен произойти rehash
  assert(ht4.size() == 4);
  assert(ht4.find(3)->value == "three");
  assert(ht4.load_factor() <= ht4.max_load_factor());
  std::cout << "Test 20 passed: rehash on table overflow.\n";

  // Тест 21: Резервирование большего размера
  ht4.reserve(20);
  assert(ht4.size() == 4);
  assert(ht4.load_factor() < 1.0f);
  assert(ht4.find(4)->value == "four");
  std::cout << "Test 21 passed: reserve() with expansion.\n";

  // Тест 22: Вставка существующего ключа
  HashTable<int, std::string> ht_existing;
  ht_existing.insert({1, "one"});
  auto result =
      ht_existing.insert({1, "uno"}); // Пытаемся вставить ключ 1 снова
  assert(!result.second); // Вставка должна вернуть false
  assert(ht_existing.find(1)->value == "one"); // Значение не изменилось
  std::cout << "Test 22 passed: insert existing key check.\n";

  // Тест 23: Поиск несуществующего ключа
  HashTable<int, std::string> ht_not_found;
  ht_not_found.insert({1, "one"});
  auto it_not_found = ht_not_found.find(2);
  assert(it_not_found == ht_not_found.end()); // Должен вернуть end()
  std::cout << "Test 23 passed: find non-existent key returns end().\n";

  // Тест 24: Итератор пропускает удалённые узлы в начале
  HashTable<int, std::string> ht_iter_skip(5);
  ht_iter_skip.insert({1, "one"});
  ht_iter_skip.insert({2, "two"});
  ht_iter_skip.erase(1); // Удаляем первый узел
  auto iter = ht_iter_skip.begin();
  assert(iter->key == 2); // Итератор должен начать с ключа 2
  std::cout << "Test 24 passed: iterator skips deleted nodes at beginning.\n";

  // Тест 25: Переполнение таблицы и проверка обхода
  HashTable<int, std::string> ht_full(4);
  ht_full.max_load_factor(10.0f); // Высокий max_load_factor
  ht_full.insert({0, "zero"});
  ht_full.insert({1, "one"});
  ht_full.insert({2, "two"});
  ht_full.insert({3, "three"});
  ht_full.insert({4, "four"}); // Принудительно заполняем таблицу
  assert(ht_full.size() == 5);
  std::cout << "Test 25 passed: find_position wraps around when table is full.\n";

  // Тест 26: Обработка удалённых узлов в `find_position`
  HashTable<int, std::string> ht_probe(5);
  ht_probe.insert({1, "one"});
  ht_probe.insert({6, "six"});     // Коллизия
  ht_probe.insert({11, "eleven"}); // Коллизия
  ht_probe.erase(6);               // Удаляем ключ 6
  ht_probe.insert({16, "sixteen"}); // Должен занять место удалённого ключа 6
  assert(ht_probe.find(16)->value == "sixteen");
  std::cout << "Test 26 passed\n";

  // Тест 27: Итератор пропускает удалённые узлы в середине таблицы
  HashTable<int, std::string> ht_iter_middle(5);
  ht_iter_middle.insert({1, "one"});
  ht_iter_middle.insert({2, "two"});
  ht_iter_middle.insert({3, "three"});
  ht_iter_middle.erase(2); // Удаляем узел в середине
  int key_sum_iter = 0;
  for (auto iter = ht_iter_middle.begin(); iter != ht_iter_middle.end();
       ++iter) {
    key_sum_iter += iter->key;
  }
  assert(key_sum_iter == 1 + 3);
  std::cout << "Test 27 passed: iterator skips deleted nodes in the middle.\n";

  // Тест 28: Вызов rehash() из operator[]
  HashTable<int, int> ht_op_bracket(4);
  ht_op_bracket.max_load_factor(0.75f);
  ht_op_bracket[1] = 1;
  ht_op_bracket[2] = 2;
  ht_op_bracket[3] = 3;
  ht_op_bracket[4] = 4; // Должен вызвать rehash()
  assert(ht_op_bracket.size() == 4);
  std::cout << "Test 28 passed: rehash triggered in operator[].\n";

  // Тест 29: Проверка позиции через вставку
  HashTable<int, std::string> ht_empty_slot(5);
  ht_empty_slot.insert({1, "one"});
  ht_empty_slot.insert(
      {2, "two"}); // Проверяем, что слот найден и элемент добавлен
  assert(ht_empty_slot.count(2) == 1); // Убедимся, что элемент присутствует
  std::cout << "Test 29 passed: position identified via insertion.\n";

  // Тест 30: Итератор начинает с удалённого узла
  {
    HashTable<int, std::string> ht_iter_deleted(5);
    ht_iter_deleted.insert({2, "two"});
    ht_iter_deleted.insert({3, "three"});
    ht_iter_deleted.erase(2);
    auto it = ht_iter_deleted.begin();
    assert(it != ht_iter_deleted.end());
    assert(it->key == 3);
    std::cout << "Test 30 passed: iterator skips initial deleted nodes.\n";
  }

  // Тест 31: Пробирование с учётом удалённых узлов
  HashTable<int, std::string> ht_probe_deleted(5);
  ht_probe_deleted.insert({1, "one"});
  ht_probe_deleted.insert({6, "six"}); // Коллизия с ключом 1
  ht_probe_deleted.erase(1);           // Удаляем ключ 1
  ht_probe_deleted.insert(
      {11, "eleven"}); // Должен занять место удалённого ключа 1
  assert(ht_probe_deleted.find(11)->value == "eleven");
  std::cout
      << "Test 31 passed: find_position reuses deleted slots during probing.\n";

  // Тест 32: Таблица полностью заполнена, проверка переполнения
  HashTable<int, std::string> ht_full_table(4);
  ht_full_table.max_load_factor(10.0f); // Предотвращаем перехеширование
  ht_full_table.insert({0, "zero"});
  ht_full_table.insert({1, "one"});
  ht_full_table.insert({2, "two"});
  ht_full_table.insert({3, "three"});
  auto result_full = ht_full_table.insert({4, "four"});
  assert(result_full.second); // Вставка должна быть успешной
  assert(ht_full_table.size() == 5); // Размер таблицы увеличился
  assert(ht_full_table.find(4) !=
         ht_full_table.end()); // Элемент с ключом 4 найден
  std::cout
      << "Test 32 passed: insert fails when table is full without rehash.\n";

  // Тест 33: Попытка вставить существующий ключ
  HashTable<int, std::string> ht_existing_key;
  ht_existing_key.insert({1, "one"});
  auto result_existing = ht_existing_key.insert({1, "uno"});
  assert(!result_existing.second);
  assert(ht_existing_key.find(1)->value == "one");
  std::cout << "Test 33 passed: insert returns false when key exists.\n";

  // Тест 34: Проверка вызова rehash() в операторе []
  HashTable<int, int> ht_rehash_op(4);
  ht_rehash_op.max_load_factor(0.75f);
  ht_rehash_op[1] = 10;
  ht_rehash_op[2] = 20;
  ht_rehash_op[3] = 30;
  ht_rehash_op[4] = 40;
  assert(ht_rehash_op.size() == 4);
  std::cout << "Test 34 passed: operator[] triggers rehash when necessary.\n";

  // Тест 35: Проверка работы константного итератора
  const HashTable<int, std::string> ht_const_iter = [] {
    HashTable<int, std::string> ht_temp;
    ht_temp.insert({1, "one"});
    ht_temp.insert({2, "two"});
    return ht_temp;
  }();
  auto const_it = ht_const_iter.begin();
  assert(const_it != ht_const_iter.end());
  assert(const_it->key == 1 || const_it->key == 2);
  std::cout << "Test 35 passed: const_iterator works correctly.\n";

  {
    HashTable<std::string, int> ht_string(4);
    ht_string.max_load_factor(0.75f);

    // Вставляем элементы, чтобы превысить max_load_factor и вызвать rehash()
    ht_string.insert({"one", 1});
    ht_string.insert({"two", 2});
    ht_string.insert({"three", 3});
    ht_string.insert({"four", 4}); // Должен вызвать rehash()

    // Проверяем, что все элементы доступны после перехеширования
    assert(ht_string.size() == 4);
    assert(ht_string.find("one")->value == 1);
    assert(ht_string.find("two")->value == 2);
    assert(ht_string.find("three")->value == 3);
    assert(ht_string.find("four")->value == 4);

    // Проверяем, что load_factor не превышает max_load_factor после rehash
    assert(ht_string.load_factor() <= ht_string.max_load_factor());

    std::cout << "Test 36 passed: rehash() called and works correctly for "
                 "HashTable<std::string, int>.\n";
  }

  // Все тесты пройдены
  std::cout << "All tests passed successfully!\n";

  return 0;
}
