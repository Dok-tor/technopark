// hashtable.hpp
#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include <functional>
#include <iterator>
#include <utility>
#include <vector>

// Реализация хэш-таблицы с открытой адресацией и линейным пробированием
template <typename KeyType, typename ValueType,
          typename HashT = std::hash<KeyType>>
class HashTable {
public:
  // Узел хэш-таблицы
  struct HashNode {
    KeyType key;
    ValueType value;
    bool occupied; // Занят ли слот
    bool deleted;  // Был ли элемент удален

    HashNode() : occupied(false), deleted(false) {}
    HashNode(KeyType k, ValueType v, bool occ)
        : key(std::move(k)), value(std::move(v)), occupied(occ),
          deleted(false) {}
  };

  // Итератор для хэш-таблицы
  class iterator {
  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = HashNode;
    using difference_type = std::ptrdiff_t;
    using pointer = HashNode *;
    using reference = HashNode &;

    iterator(HashNode *ptr, HashNode *end_ptr)
        : node_ptr(ptr), end_ptr(end_ptr) {
      // Пропускаем пустые или удаленные узлы
      while (node_ptr != end_ptr &&
             (!node_ptr->occupied || node_ptr->deleted)) {
        ++node_ptr;
      }
    }

    reference operator*() const { return *node_ptr; }

    pointer operator->() const { return node_ptr; }

    iterator &operator++() {
      do {
        ++node_ptr;
      } while (node_ptr != end_ptr &&
               (!node_ptr->occupied || node_ptr->deleted));
      return *this;
    }

    iterator operator++(int) {
      iterator temp = *this;
      ++*this;
      return temp;
    }

    bool operator==(const iterator &other) const {
      return node_ptr == other.node_ptr;
    }

    bool operator!=(const iterator &other) const {
      return node_ptr != other.node_ptr;
    }

  private:
    HashNode *node_ptr;
    HashNode *end_ptr;
  };

  // Константный итератор
  class const_iterator {
  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = const HashNode;
    using difference_type = std::ptrdiff_t;
    using pointer = const HashNode *;
    using reference = const HashNode &;

    const_iterator(const HashNode *ptr, const HashNode *end_ptr)
        : node_ptr(ptr), end_ptr(end_ptr) {
      while (node_ptr != end_ptr &&
             (!node_ptr->occupied || node_ptr->deleted)) {
        ++node_ptr;
      }
    }

    reference operator*() const { return *node_ptr; }

    pointer operator->() const { return node_ptr; }

    const_iterator &operator++() {
      do {
        ++node_ptr;
      } while (node_ptr != end_ptr &&
               (!node_ptr->occupied || node_ptr->deleted));
      return *this;
    }

    const_iterator operator++(int) {
      const_iterator temp = *this;
      ++*this;
      return temp;
    }

    bool operator==(const const_iterator &other) const {
      return node_ptr == other.node_ptr;
    }

    bool operator!=(const const_iterator &other) const {
      return node_ptr != other.node_ptr;
    }

  private:
    const HashNode *node_ptr;
    const HashNode *end_ptr;
  };

  // Конструктор
  explicit HashTable(size_t capacity = initial_capacity)
      : num_elements(0), max_load_factor_(0.75f) {
    table.resize(capacity);
  }

  // Итераторы
  iterator begin() { return iterator(&table[0], &table[0] + table.size()); }

  iterator end() {
    return iterator(&table[0] + table.size(), &table[0] + table.size());
  }

  const_iterator begin() const {
    return const_iterator(&table[0], &table[0] + table.size());
  }

  const_iterator end() const {
    return const_iterator(&table[0] + table.size(), &table[0] + table.size());
  }

  // Проверка на пустоту
  bool empty() const { return num_elements == 0; }

  // Количество элементов
  size_t size() const { return num_elements; }

  // Очистка таблицы
  void clear() {
    table.clear();
    table.resize(initial_capacity);
    num_elements = 0;
  }

  // Резервирование памяти
  void reserve(size_t new_capacity) {
    if (new_capacity > table.size()) {
      rehash(new_capacity);
    }
  }

  // Фактор загрузки
  float load_factor() const {
    return static_cast<float>(num_elements) / table.size();
  }

  // Максимальный фактор загрузки
  float max_load_factor() const { return max_load_factor_; }

  void max_load_factor(float ml) { max_load_factor_ = ml; }

  // Оператор []
  ValueType& operator[](const KeyType& key) {
    if (load_factor() > max_load_factor_) {
        rehash();
    }
    size_t index = find_position(key);
    if (!table[index].occupied || table[index].deleted) {
        table[index] = HashNode(key, ValueType(), true);
        ++num_elements;
    }
    return table[index].value;
}

  // Поиск элемента
  iterator find(const KeyType &key) {
    size_t index = find_position(key);
    if (table[index].occupied && !table[index].deleted) {
      return iterator(&table[index], &table[0] + table.size());
    }
    return end();
  }

  const_iterator find(const KeyType &key) const {
    size_t index = find_position(key);
    if (table[index].occupied && !table[index].deleted) {
      return const_iterator(&table[index], &table[0] + table.size());
    }
    return end();
  }

  // Проверка наличия ключа
  size_t count(const KeyType &key) const {
    return (find(key) != end()) ? 1 : 0;
  }

  // Вставка элемента
  std::pair<iterator, bool> insert(const std::pair<KeyType, ValueType> &pair) {
    if (load_factor() > max_load_factor_) {
      rehash();
    }
    size_t index = find_position(pair.first);
    if (table[index].occupied && !table[index].deleted) {
      return {iterator(&table[index], &table[0] + table.size()), false};
    }
    table[index] = HashNode(pair.first, pair.second, true);
    ++num_elements;
    return {iterator(&table[index], &table[0] + table.size()), true};
  }

  std::pair<iterator, bool> insert(std::pair<KeyType, ValueType> &&pair) {
    if (load_factor() > max_load_factor_) {
      rehash();
    }
    size_t index = find_position(pair.first);
    if (table[index].occupied && !table[index].deleted) {
      return {iterator(&table[index], &table[0] + table.size()), false};
    }
    table[index] =
        HashNode(std::move(pair.first), std::move(pair.second), true);
    ++num_elements;
    return {iterator(&table[index], &table[0] + table.size()), true};
  }

  // Удаление элемента
  size_t erase(const KeyType &key) {
    size_t index = find_position(key);
    if (table[index].occupied && !table[index].deleted) {
      table[index].deleted = true;
      --num_elements;
      return 1;
    }
    return 0;
  }

private:
  std::vector<HashNode> table;
  size_t num_elements;
  float max_load_factor_;
  HashT hash_function;
  static constexpr size_t initial_capacity = 16;

  // Поиск позиции для ключа
  size_t find_position(const KeyType &key) const {
    size_t index = hash_function(key) % table.size();
    size_t start_index = index;
    size_t first_deleted_index =
        table.size(); // Индекс первого удаленного слота

    while (table[index].occupied) {
      if (!table[index].deleted) {
        if (table[index].key == key) {
          return index; // Ключ найден
        }
      } else {
        if (first_deleted_index == table.size()) {
          first_deleted_index = index; // Запоминаем первый удаленный слот
        }
      }
      index = (index + 1) % table.size();
      if (index == start_index) {
        break; // Таблица заполнена
      }
    }
    // Возвращаем первый удаленный слот или текущий свободный
    return (first_deleted_index != table.size()) ? first_deleted_index : index;
  }

  // Перехеширование таблицы
  void rehash(size_t new_capacity = 0) {
    size_t old_capacity = table.size();
    size_t capacity = new_capacity ? new_capacity : old_capacity * 2;
    std::vector<HashNode> old_table = std::move(table);
    table.clear();
    table.resize(capacity);
    num_elements = 0;

    for (auto &node : old_table) {
      if (node.occupied && !node.deleted) {
        insert(std::make_pair(std::move(node.key), std::move(node.value)));
      }
    }
  }
};

#endif // HASHTABLE_HPP
