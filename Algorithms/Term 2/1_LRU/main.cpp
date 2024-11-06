#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <sstream>
#include <limits> // Добавляем этот заголовочный файл

// Реализация хэш-таблицы с открытой адресацией и линейным пробированием
template <typename KeyType, typename ValueType, typename HashT = std::hash<KeyType>>
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
                : key(std::move(k)), value(std::move(v)), occupied(occ), deleted(false) {}
    };

    // Конструкторы
    explicit HashTable(size_t capacity = initial_capacity)
            : num_elements(0), max_load_factor_(0.75f) {
        table.resize(capacity);
    }

    // Итераторы для хэш-таблицы
    class iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = HashNode;
        using difference_type = std::ptrdiff_t;
        using pointer = HashNode*;
        using reference = HashNode&;

        iterator(HashNode* ptr, HashNode* end_ptr) : node_ptr(ptr), end_ptr(end_ptr) {
            // Пропускаем пустые или удаленные узлы
            while (node_ptr != end_ptr && (!node_ptr->occupied || node_ptr->deleted)) {
                ++node_ptr;
            }
        }

        reference operator*() const {
            return *node_ptr;
        }

        pointer operator->() const {
            return node_ptr;
        }

        iterator& operator++() {
            do {
                ++node_ptr;
            } while (node_ptr != end_ptr && (!node_ptr->occupied || node_ptr->deleted));
            return *this;
        }

        iterator operator++(int) {
            iterator temp = *this;
            ++*this;
            return temp;
        }

        bool operator==(const iterator& other) const {
            return node_ptr == other.node_ptr;
        }

        bool operator!=(const iterator& other) const {
            return node_ptr != other.node_ptr;
        }

    private:
        HashNode* node_ptr;
        HashNode* end_ptr;
    };

    // Методы доступа к итераторам
    iterator begin() {
        return iterator(&table[0], &table[0] + table.size());
    }

    iterator end() {
        return iterator(&table[0] + table.size(), &table[0] + table.size());
    }

    // Проверка на пустоту
    bool empty() const {
        return num_elements == 0;
    }

    // Количество элементов
    size_t size() const {
        return num_elements;
    }

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
    float max_load_factor() const {
        return max_load_factor_;
    }

    void max_load_factor(float ml) {
        max_load_factor_ = ml;
    }

    // Оператор []
    ValueType& operator[](const KeyType& key) {
        size_t index = find_position(key);
        if (!table[index].occupied || table[index].deleted) {
            // Вставляем новый элемент по умолчанию
            if (load_factor() > max_load_factor_) {
                rehash();
                index = find_position(key);
            }
            table[index] = HashNode(key, ValueType(), true);
            ++num_elements;
        }
        return table[index].value;
    }

    // Вставка элемента
    std::pair<iterator, bool> insert(const std::pair<KeyType, ValueType>& pair) {
        return insert(std::pair<KeyType, ValueType>(pair));
    }

    std::pair<iterator, bool> insert(std::pair<KeyType, ValueType>&& pair) {
        if (load_factor() > max_load_factor_) {
            rehash();
        }
        size_t index = find_position(pair.first);
        if (table[index].occupied && !table[index].deleted) {
            return { iterator(&table[index], &table[0] + table.size()), false };
        }
        table[index] = HashNode(std::move(pair.first), std::move(pair.second), true);
        ++num_elements;
        return { iterator(&table[index], &table[0] + table.size()), true };
    }

    // Поиск элемента
    iterator find(const KeyType& key) {
        size_t index = find_position(key);
        if (table[index].occupied && !table[index].deleted) {
            return iterator(&table[index], &table[0] + table.size());
        }
        return end();
    }

    // Проверка наличия ключа
    size_t count(const KeyType& key) {
        return (find(key) != end()) ? 1 : 0;
    }

    // Удаление элемента
    size_t erase(const KeyType& key) {
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
    size_t find_position(const KeyType& key) const {
        size_t index = hash_function(key) % table.size();
        size_t start_index = index;
        size_t first_deleted_index = table.size(); // Индекс первого удаленного слота

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

        for (auto& node : old_table) {
            if (node.occupied && !node.deleted) {
                insert(std::make_pair(std::move(node.key), std::move(node.value)));
            }
        }
    }
};

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
    bool Put(KeyType key, ValueType value) {
        size_t item_size = key.size() + sizeof(value[0]) * value.size();
        if (item_size > max_bytes) {
            return false; // Элемент не влезает в кэш
        }
        auto it = cache_map.find(key);
        if (it != cache_map.end()) {
            // Обновляем существующий элемент
            cache_list.splice(cache_list.begin(), cache_list, it->value);
            current_bytes -= key.size() + sizeof(it->value->second[0]) * it->value->second.size();
            it->value->second = std::move(value);
            current_bytes += item_size;
        } else {
            // Проверяем, нужно ли удалять старые элементы
            while (cache_list.size() >= capacity || (current_bytes + item_size) > max_bytes) {
                auto& last = cache_list.back();
                current_bytes -= last.first.size() + sizeof(last.second[0]) * last.second.size();
                cache_map.erase(last.first);
                cache_list.pop_back();
            }
            // Вставляем новый элемент
            cache_list.emplace_front(std::move(key), std::move(value));
            cache_map.insert(std::make_pair(cache_list.front().first, cache_list.begin()));
            current_bytes += item_size;
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
    size_t capacity;   // Максимальное количество элементов
    size_t max_bytes;  // Максимальный размер в байтах
    size_t current_bytes; // Текущий размер в байтах

    // Список для отслеживания порядка использования
    std::list<std::pair<KeyType, ValueType>> cache_list;

    // Хэш-таблица для быстрого доступа к элементам
    HashTable<KeyType, typename std::list<std::pair<KeyType, ValueType>>::iterator> cache_map;
};

// Проверка валидности строки (кодировка UTF-8 или CP1251)
bool isValidString(const std::string& str) {
    // Проверяем, что строка не содержит запрещенных символов
    for (unsigned char c : str) {
        if (c == '\n' || c == '\r' || c == '\t' || c == '\0' || c == ' ') {
            return false;
        }
        if (c >= 0x80 && c <= 0xBF) {
            // Запрещенные символы в CP1251
            return false;
        }
    }
    // В реальном коде здесь можно использовать ICU для проверки кодировки
    return true;
}

// Обработка входных данных
void processInput(const std::string& input, LRUCache<std::string, std::vector<float>>& cache) {
    size_t tab_pos = input.find('\t');
    if (tab_pos != std::string::npos) {
        // Строка типа (а): <q>\t<f1> <f2> <f3> ...
        std::string q = input.substr(0, tab_pos);
        std::string floats_str = input.substr(tab_pos + 1);

        if (!isValidString(q)) {
            std::cout << "!STORERR!" << std::endl;
            return;
        }

        std::istringstream iss(floats_str);
        std::vector<float> embedding;
        float value;

        while (iss >> value) {
            embedding.push_back(value);
        }

        if (embedding.size() != 64 || !iss.eof()) {
            // Если не удалось считать ровно 64 числа или есть лишние данные
            std::cout << "!STORERR!" << std::endl;
            return;
        }

        if (cache.Put(std::move(q), std::move(embedding))) {
            std::cout << "!STORED!" << std::endl;
        } else {
            std::cout << "!STORERR!" << std::endl;
        }
    } else {
        // Строка типа (б): <q>
        const std::string& q = input;

        if (!isValidString(q)) {
            std::cout << "!NOEMBED!" << std::endl;
            return;
        }

        std::vector<float>* result = cache.Get(q);
        if (result) {
            for (size_t i = 0; i < result->size(); ++i) {
                std::cout << (*result)[i];
                if (i != result->size() - 1) {
                    std::cout << " ";
                }
            }
            std::cout << std::endl;
        } else {
            std::cout << "!NOEMBED!" << std::endl;
        }
    }
}

int main() {
    size_t N = 3;      // Максимальный размер кэша в записях
    size_t Size = 1000;   // Максимальный размер кэша в байтах

    // Читаем значения N и Size
    std::cin >> N >> Size;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    LRUCache<std::string, std::vector<float>> cache(N, Size);
    std::string line;
    while (std::getline(std::cin, line)) {
        processInput(line, cache);
    }
    return 0;
}
