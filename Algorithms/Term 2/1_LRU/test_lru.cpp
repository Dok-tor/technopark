// test_lru.cpp
#include "lru.hpp"
#include <cassert>
#include <iostream>
#include <string>

class KeyWithSize {
public:
  KeyWithSize() : key("") {} // Добавлен конструктор по умолчанию
  KeyWithSize(const std::string &k) : key(k) {}
  size_t size() const { return key.size(); }
  bool operator==(const KeyWithSize &other) const { return key == other.key; }
  std::string key;
};

// Специализация std::hash для KeyWithSize
namespace std {
template <> struct hash<KeyWithSize> {
  size_t operator()(const KeyWithSize &k) const {
    return std::hash<std::string>()(k.key);
  }
};
} // namespace std

int main() {
  std::cout << "Starting LRUCache tests...\n";

  // Тест 1: Проверка конструктора и методов size()
  LRUCache<KeyWithSize, std::string> cache(3, 100);
  assert(cache.size() == 0);
  assert(cache.size_bytes() == 0);
  std::cout << "Test 1 passed: Constructor and size() checks.\n";

  // Тест 2: Проверка метода Put() и добавления элементов
  cache.Put(KeyWithSize("key1"), "value1");
  cache.Put(KeyWithSize("key2"), "value2");
  cache.Put(KeyWithSize("key3"), "value3");
  assert(cache.size() == 3);
  std::cout << "Test 2 passed: Put() and adding elements.\n";

  // Тест 3: Проверка метода Get()
  auto val = cache.Get(KeyWithSize("key2"));
  assert(val != nullptr);
  assert(*val == "value2");
  std::cout << "Test 3 passed: Get() method check.\n";

  // Тест 4: Проверка обновления элемента
  cache.Put(KeyWithSize("key2"), "value2_updated");
  val = cache.Get(KeyWithSize("key2"));
  assert(val != nullptr);
  assert(*val == "value2_updated");
  std::cout << "Test 4 passed: Updating existing element.\n";

  // Тест 5: Проверка удаления наименее используемого элемента при превышении
  // capacity
  cache.Put(KeyWithSize("key4"), "value4");
  assert(cache.size() == 3); // Capacity = 3
  val = cache.Get(KeyWithSize("key1"));
  assert(val == nullptr); // "key1" должен быть удален
  std::cout << "Test 5 passed: Eviction when capacity exceeded.\n";

  // Тест 6: Проверка порядка использования (LRU)
  cache.Get(KeyWithSize("key2"));
  cache.Put(KeyWithSize("key5"), "value5");
  val = cache.Get(KeyWithSize("key3"));
  assert(val == nullptr); // "key3" должен быть удален
  std::cout << "Test 6 passed: LRU order check.\n";

  // Тест 7: Проверка ограничения по max_bytes
  // Создаем ключи и значения большого размера
  std::string large_key = "large_key_with_size";
  std::string large_value = "large_value_with_more_size";
  size_t large_item_size = large_key.size() + large_value.size();

  LRUCache<KeyWithSize, std::string> cache_bytes(10, large_item_size * 2);
  cache_bytes.Put(KeyWithSize(large_key),
                  large_value); // Вставляем первый большой элемент
  cache_bytes.Put(KeyWithSize("small_key"),
                  "small_value"); // Вставляем маленький элемент
  assert(cache_bytes.size() == 2);

  // Делаем "small_key" наиболее недавно использованным
  cache_bytes.Get(KeyWithSize("small_key"));

  // Вставляем второй большой элемент
  cache_bytes.Put(KeyWithSize("another_large_key"), large_value);
  assert(cache_bytes.size() == 2); // Должен был удалить "large_key_with_size"

  // Проверяем, что "large_key_with_size" был удален
  auto val_1 = cache_bytes.Get(KeyWithSize(large_key));
  assert(val_1 == nullptr); // "large_key_with_size" должен быть удален

  // Проверяем, что "small_key" все еще в кэше
  val_1 = cache_bytes.Get(KeyWithSize("small_key"));
  assert(val_1 != nullptr);
  assert(*val_1 == "small_value");

  std::cout << "Test 7 passed: Max bytes limit check.\n";

  // Тест 8: Проверка метода clear()
  cache.clear();
  assert(cache.size() == 0);
  assert(cache.size_bytes() == 0);
  std::cout << "Test 8 passed: clear() method check.\n";

  // Тест 9: Проверка работы с ValueType без метода size()
  class ValueWithoutSize {
  public:
    ValueWithoutSize(int v) : value(v) {}
    int value;
  };

  LRUCache<KeyWithSize, ValueWithoutSize> cache_no_size(3, 100);
  cache_no_size.Put(KeyWithSize("key1"), ValueWithoutSize(1));
  cache_no_size.Put(KeyWithSize("key2"), ValueWithoutSize(2));
  cache_no_size.Put(KeyWithSize("key3"), ValueWithoutSize(3));
  assert(cache_no_size.size() == 3);
  auto val_no_size = cache_no_size.Get(KeyWithSize("key2"));
  assert(val_no_size != nullptr);
  assert(val_no_size->value == 2);
  std::cout << "Test 9 passed: Handling ValueType without size() method.\n";

  // Тест 10: Проверка отказа в вставке элемента, превышающего max_bytes
  LRUCache<KeyWithSize, std::string> cache_small(
      3, 10); // Очень маленький max_bytes
  bool put_result =
      cache_small.Put(KeyWithSize("big_key"), "big_value_exceeding_max_bytes");
  assert(!put_result);
  assert(cache_small.size() == 0);
  std::cout
      << "Test 10 passed: Insertion refusal when item exceeds max_bytes.\n";

  // Тест 11: Проверка удаления всех элементов при вставке нового большого
  // элемента LRUCache<KeyWithSize, std::string> cache_small(3, 10); //
  // capacity=3, max_bytes=10

  cache_small.Put(KeyWithSize("a"), "b"); // Размер: 1 + 1 = 2 байта
  cache_small.Put(KeyWithSize("c"), "d"); // Размер: 1 + 1 = 2 байта
  cache_small.Put(KeyWithSize("e"), "f"); // Размер: 1 + 1 = 2 байта
  assert(cache_small.size() == 3); // Суммарный размер: 6 байт

  put_result =
      cache_small.Put(KeyWithSize("big_key"), "big_value_exceeding_max_bytes");
  assert(!put_result);
  assert(cache_small.size() == 3); // Размер кэша должен остаться 3
  std::cout << "Test 11 passed: No eviction when new item cannot fit.\n";

  // Тест 12: Проверка корректности current_bytes
  size_t initial_bytes = cache_bytes.size_bytes();
  cache_bytes.Put(KeyWithSize("small_key2"), "small_value2");
  assert(cache_bytes.size_bytes() > initial_bytes);
  std::cout << "Test 12 passed: current_bytes tracking check.\n";

  // Тест 13: Проверка повторного добавления удаленного элемента
  cache.Put(KeyWithSize("key1"), "value1");
  cache.Put(KeyWithSize("key2"), "value2");
  cache.Put(KeyWithSize("key3"), "value3");
  cache.Get(
      KeyWithSize("key1")); // Делаем "key1" наиболее недавно использованным
  cache.Put(KeyWithSize("key4"), "value4"); // Должен удалить "key2"
  cache.Put(KeyWithSize("key2"),
            "value2_new"); // Пытаемся добавить "key2" снова
  val = cache.Get(KeyWithSize("key2"));
  assert(val != nullptr);
  assert(*val == "value2_new");
  std::cout << "Test 13 passed: Re-insertion of evicted element.\n";

  // Тест 14: Проверка работы с большим количеством элементов
  LRUCache<KeyWithSize, int> cache_large(1000, 100000);
  for (int i = 0; i < 1000; ++i) {
    cache_large.Put(KeyWithSize("key" + std::to_string(i)), i);
  }
  assert(cache_large.size() == 1000);
  bool data_correct = true;
  for (int i = 0; i < 1000; ++i) {
    auto val_large = cache_large.Get(KeyWithSize("key" + std::to_string(i)));
    if (val_large == nullptr || *val_large != i) {
      data_correct = false;
      break;
    }
  }
  assert(data_correct);
  std::cout << "Test 14 passed: Large dataset handling.\n";

  // Тест 15: Проверка ситуации, когда capacity == 0
  LRUCache<KeyWithSize, std::string> cache_zero(0, 100);
  put_result = cache_zero.Put(KeyWithSize("key"), "value");
  assert(!put_result);
  assert(cache_zero.size() == 0);
  std::cout << "Test 15 passed: Handling cache with zero capacity.\n";

  // Все тесты пройдены
  std::cout << "All LRUCache tests passed successfully!\n";

  return 0;
}
