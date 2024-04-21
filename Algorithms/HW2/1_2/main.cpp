// Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией.
// Хранимые строки непустые и состоят из строчных латинских букв.
// Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера.
// Начальный размер таблицы должен быть равным 8-ми.
// Перехеширование выполняйте при добавлении элементов в случае, когда коэффициент заполнения таблицы достигает 3/4.
// Структура данных должна поддерживать операции добавления строки в множество,
// удаления строки из множества и проверки принадлежности данной строки множеству.
//
// Вариант 2. Для разрешения коллизий используйте двойное хеширование.


#include <iostream>
#include <vector>

const size_t DEFAULT_SIZE = 8;

const std::string DEL = "D";
const std::string EMPTY = "E";

const size_t DEL_NOT_FOUND = -1;

class StringHasher {
public:
    explicit StringHasher(size_t prime = 11) : prime(prime) {}
    size_t operator ()(const std::string &str) const
    {
        size_t hash = 0;
        for (int i = 0; i < str.size(); i++)
        {
            hash = hash * prime + str[i];
        }
        return hash;
    }
private:
    size_t prime;
};

class StringHasher_71 : public StringHasher {
public:
    StringHasher_71() : StringHasher(71) {}
};

class StringHasher_43 : public StringHasher {
public:
    StringHasher_43() : StringHasher(43) {}
};


template <typename T, typename Hasher1, typename Hasher2>
class Set {
public:
    explicit Set(size_t initial_size = DEFAULT_SIZE) : count_keys(0), table(initial_size, EMPTY) {}

    Set(const Set&) = delete;

    Set& operator=(const Set&) = delete;

    ~Set() = default;

    bool Has(const T &key) {
        for (size_t i = 0; i < table.size(); ++i) {
            size_t hash = Hash(key, i, table.size());
            if (table[hash] == key) {
                return true;
            }
            if (table[hash] == EMPTY) {
                return  false;
            }
        }
        return false;
    }

    bool Add(const T &key) {
        if (static_cast<double>(count_keys)  / static_cast<double>(table.size()) >= 3.0 / 4.0) {
            ReHash();
        }

        size_t DEL_pos = DEL_NOT_FOUND;

        for (size_t i = 0; i < table.size(); ++i) {
            size_t hash = Hash(key, i, table.size());

            if (table[hash] == key) {
                return false;
            }

            if (table[hash] == DEL and DEL_pos == DEL_NOT_FOUND) {
                DEL_pos = hash;
                continue;
            }

            if (table[hash] == EMPTY) {

                if (DEL_pos == DEL_NOT_FOUND) {
                    table[hash] = key;
                } else {
                    table[DEL_pos] = key;
                }
                count_keys++;
                return true;
            }
        }

        if (DEL_pos == DEL_NOT_FOUND) {
            return false;
        }
        table[DEL_pos] = key;
        count_keys++;
        return true;
    }

    bool Delete(const T &key) {
        for (size_t i = 0; i < table.size(); ++i) {
            size_t hash = Hash(key, i, table.size());

            if (table[hash] == key) {
                table[hash] = DEL;
                count_keys--;
                return true;
            }

            if (table[hash] == EMPTY) {
                return false;
            }
        }
        return false;
    }

private:
    size_t Hash(const T &key, size_t i, size_t n) {
        return (h1(key) + (2 * h2(key) + 1) * i) % n;
    }

    void ReHash() {
        std::vector<T> newTable(table.size() * 2, EMPTY);

        for (size_t i = 0; i < table.size(); ++i) {
            if (table[i] == EMPTY or table[i] == DEL) {
                continue;
            }

            for (size_t j = 0; j < newTable.size(); ++j) {
                size_t hash = Hash(table[i], j, newTable.size());

                if (newTable[hash] == EMPTY) {
                    newTable[hash] = table[i];
                    break;
                }
            }
        }
        std::swap(newTable, table);
    }

    std::vector<T> table;
    size_t count_keys = 0;
    Hasher1 h1;
    Hasher2 h2;
};



void process() {

    Set<std::string, StringHasher_71, StringHasher_43> table;
    char op;
    std::string key;

    while (std::cin >> op >> key)
    {
        switch (op)
        {
            case '?':
            {
                std::cout << (table.Has(key) ? "OK" : "FAIL") << std::endl;
                break;
            }
            case '+':
            {
                std::cout << (table.Add(key) ? "OK" : "FAIL") << std::endl;
                break;
            }
            case '-':
            {
                std::cout << (table.Delete(key) ? "OK" : "FAIL") << std::endl;
                break;
            }
            default:
            {
                return;
            }
        }
    }
}


int main() {
    process();
    return 0;
}
