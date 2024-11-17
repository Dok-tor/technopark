#ifndef LRU_HPP
#define LRU_HPP

#include "hashtable.hpp"
#include <list>
#include <vector>
#include <cstddef>

// Реализация LRU-кэша
template <typename KeyType, typename ValueType>
class LRUCache {
public:
    explicit LRUCache(size_t capacity, size_t max_bytes)
        : capacity(capacity), max_bytes(max_bytes), current_bytes(0) {}

    // Получение значения по ключу
    ValueType* Get(const KeyType& key) {
        auto it = cache_map.find(key);
        if (it == cache_map.end()) {
            return nullptr; // Элемент не найден
        }
        // Перемещаем элемент в начало списка (обновляем "свежесть" доступа)
        cache_list.splice(cache_list.begin(), cache_list, it->value);
        return &(it->value->second);
    }

    // Вставка или обновление элемента
    bool Put(const KeyType& key, const ValueType& value) {
        size_t new_item_size = item_size(key, value);
        if (new_item_size > max_bytes || capacity == 0) {
            return false; // Элемент не влезает в кэш
        }
        auto it = cache_map.find(key);
        if (it != cache_map.end()) {
            // Обновляем существующий элемент
            cache_list.splice(cache_list.begin(), cache_list, it->value);
            current_bytes -= item_size(it->value->first, it->value->second);
            it->value->second = value;
            current_bytes += new_item_size;
        } else {
            // Проверяем, нужно ли удалять старые элементы
            while (cache_list.size() >= capacity || (current_bytes + new_item_size) > max_bytes) {
                auto& last = cache_list.back();
                current_bytes -= item_size(last.first, last.second);
                cache_map.erase(last.first);
                cache_list.pop_back();
            }
            // Вставляем новый элемент
            cache_list.emplace_front(key, value);
            cache_map.insert(std::make_pair(cache_list.front().first, cache_list.begin()));
            current_bytes += new_item_size;
        }
        return true;
    }

    // Размер кэша в элементах
    size_t size() const {
        return cache_list.size();
    }

    // Размер кэша в байтах
    size_t size_bytes() const {
        return current_bytes;
    }

    // Очистка кэша
    void clear() {
        cache_list.clear();
        cache_map.clear();
        current_bytes = 0;
    }

private:
    size_t capacity;       // Максимальное количество элементов
    size_t max_bytes;      // Максимальный размер в байтах
    size_t current_bytes;  // Текущий размер в байтах

    // Список для отслеживания порядка использования
    std::list<std::pair<KeyType, ValueType>> cache_list;

    // Хэш-таблица для быстрого доступа к элементам
    HashTable<KeyType, typename std::list<std::pair<KeyType, ValueType>>::iterator> cache_map;

    // Вычисление размера элемента в байтах
    size_t item_size(const KeyType& key, const ValueType& value) const {
        return key.size() + value.size();
    }
};

#endif // LRU_HPP
