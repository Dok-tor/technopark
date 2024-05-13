// Напишите две функции для создания архива из одного файла и извлечения файла из архива.
//
// Метод архивирует данные из потока original
// void Encode(IInputStream& original, IOutputStream& compressed);
// Метод восстанавливает оригинальные данные
// void Decode(IInputStream& compressed, IOutputStream& original);
// где:
// typedef char byte;
//
// interface IInputStream {
// // Возвращает false, если поток закончился
//        virtual bool Read(byte& value) = 0;
// };
//
// interface IOutputStream {
//        virtual void Write(byte value) = 0;
// };
// В архиве сохраняйте дерево Хаффмана и код Хаффмана от исходных данных.
// Дерево Хаффмана требуется хранить эффективно - не более 10 бит на каждый 8-битный символ.
//
// В контест необходимо отправить .cpp файл содержащий функции Encode, Decode, а также включающий файл Huffman.h.
// Тестирующая программа выводит размер сжатого файла в процентах от исходного.




#include <fstream>
#include <ranges>
#include <vector>
#include <unordered_map>
#include <queue>
#include <iostream>
#include <cassert>
#include <stack>
#include <algorithm>
#include <map>


/*
 * В данном задании я использовал 2 подхода к кодированию:
 * У нас имелась информация с семинара о том, что алфавит входных символов не превышает 256.
 * Оптимизации, которые были проведены по сравнению предоставленным на семинаре алгоритмом:
 * 1) Кодирования конца битовой последовательности не байтом, а тремя битами (достаточно, чтобы закодировать цифру от 0 до 7)
 * 2) Я заметил, что при записи дерева в формате 1-символ-1-символ-0-1-символ-0 в конце, когда все символы уже записаны
 *    или считаны остаются нули, которые обозначают слияние вершин до тех пор, пока не останется одна. Я посчитал, что можно их вообще не записывать
 * 3) Пользуясь веткой на StackOverflow https://stackoverflow.com/questions/759707/efficient-way-of-storing-huffman-tree
 *    в которой рассматриваются способы эффективной передачи дерева Хаффмана
 *    Вычислил, что предложенный способ записи дерева в файл эффективен только до определённого размера алфавита (10n-1, где n - размер алфавита)
 *    Тогда как кодирование в векторе (где индекс - байтовый символ, а значение - длина его битового кода) имеет фиксированный размер
 *    256 * 8 = 2048 бит. Таким образом при значении n >= 205 эффективнее сохранять таблицу длин и восстанавливать дерево при декодировании.
 *    Для выбора режима кодирования использую первый бит (1 таблица, 0 непосредственно дерево)
 * 4) Пришла идея использовать кодирование с ограничением на длину символов (например 8) тогда размер таблицы уменьшился бы более чем в 2 раза (256*3)
 *    Однако из-за трудоёмкости пришлось от этой идеи отказаться. Однако, опытным путём пронаблюдав на различных тестовых данных максимальную длину
 *    кода, я увидел, что она не превышает 32 (по крайней мере мне не удалось создать такие тестовые данные). 32 может быть закодировано 5-ю битами
 *    тогда итоговый размер таблицы получится 256 * 5 = 1280 бит. Таким образом стало эффективным использовать таблицу при
 *    размере алфавита > 128 т.к. (10 * 128 - 1 = 1279).
 * 5) Вычислил, что в некоторых случаях алгоритм сжатия производит файл значительно больше, чем исходная последовательность (за счёт дерева или таблицы)
 *    Поэтому я решил предусмотреть, чтобы в таком случае символы передавались на выход без сжатия. Для этого я добавил ещё один бит вначале
 *    '0'   - Передача без сжатия
 *    '10'  - Передача со сжатием, кодируется дерево напрямую
 *    '11'  - Передача со сжатием, кодируется таблица длин
 */

typedef unsigned char byte;
#define interface struct

interface IInputStream {
    // Возвращает false, если поток закончился
    virtual bool Read(byte& value) = 0;
};

interface IOutputStream {
    virtual void Write(byte value) = 0;
};


class FileInputStream : public IInputStream {
public:
    explicit FileInputStream(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary);
        buffer = std::vector<byte>(std::istreambuf_iterator<char>(file), {});
        position = 0;
    }

    bool Read(byte& value) override {
        if (position < buffer.size()) {
            value = buffer[position++];
            return true;
        } else {
            return false;
        }
    }

private:
    std::vector<byte> buffer;
    size_t position;
};

class FileOutputStream : public IOutputStream {
public:
    explicit FileOutputStream(const std::string& filename) {
        file.open(filename, std::ios::binary);
    }

    virtual void Write(byte value) override {
        if (file.is_open()) {
            file.write(reinterpret_cast<const char*>(&value), sizeof(byte));
        }
    }

    ~FileOutputStream() {
        if (file.is_open()) {
            file.close();
        }
    }

private:
    std::ofstream file;
};

class BitWriter {
public:
    BitWriter() : bitCount(0) {}

    void WriteBit(unsigned char bit) {
        if (bitCount % 8 == 0) {
            buffer.push_back(0);
        }
        if (bit) {
            buffer[bitCount/8] |= 1 << (7 - bitCount % 8);
        }
        bitCount++;
    }

    void WriteThreeBits(byte bits) {
        assert(bits < 8); // Трехбитовое число должно быть меньше 8
        for (int i = 2; i >= 0; --i) {
            WriteBit((bits >> i) & 1);
        }
    }

    void WriteFiveBits(byte bits) {
        assert(bits < 32); // Пятибитное число должно быть меньше 32
        for (int i = 4; i >= 0; --i) {
            WriteBit((bits >> i) & 1);
        }
    }

    void WriteByte(unsigned char byte)
    {
        if (bitCount % 8 == 0) {
            buffer.push_back(byte);
        } else {
            int offset = bitCount % 8;
            buffer[bitCount/8] |= byte >> offset;
            buffer.push_back(byte << (8 - offset));
        }
        bitCount += 8;
    }


    [[nodiscard]] const std::vector<unsigned char> &GetBuffer() const {
        return buffer;
    }

    [[nodiscard]] size_t GetBitCount() const {
        return bitCount;
    }

private:
    std::vector<unsigned char> buffer;
    size_t bitCount;
};

class BitReader {
public:
    BitReader() : bitCount(0) {}

    byte ReadBit() {
        assert(bitCount/8 < buffer.size());

        byte bit = (buffer[bitCount/8] >> (7 - bitCount % 8)) & 1;
        bitCount++;
        return bit;
    }

    void FillBuffer(IInputStream& compressed) {
        byte value;
        while (compressed.Read(value)) {
            buffer.push_back(value);
        }
    }

    byte ReadThreeBits() {
        byte result = 0;
        for (int i = 0; i < 3; ++i) {
            result = (result << 1) | ReadBit();
        }
        return result;
    }

    byte ReadFiveBits() {
        byte result = 0;
        for (int i = 0; i < 5; ++i) {
            result = (result << 1) | ReadBit();
        }
        return result;
    }

    byte ReadByte() {
        assert(bitCount/8 < buffer.size());

        byte result = buffer[bitCount/8] << (bitCount % 8);
        if (bitCount % 8 != 0 && bitCount/8 + 1 < buffer.size()) {
            result |= buffer[bitCount/8 + 1] >> (8 - bitCount % 8);
        }

        bitCount += 8;
        return result;
    }

    bool gotLast(byte last) {
        if (last == 0) {
            if (bitCount == buffer.size() * 8)
                return true;
        } else if (bitCount / 8 == buffer.size() - 1 and bitCount % 8 == last) {
            return true;
        }
        return false;
    }

    [[nodiscard]] const std::vector<unsigned char> &GetBuffer() const {
        return buffer;
    }

    [[nodiscard]] size_t GetBitCount() const {
        return bitCount;
    }

private:
    std::vector<unsigned char> buffer;
    size_t bitCount;
};


/// Реализация кучи из одного из прошлых ДЗ. (Мне просто нравится работать именно с ней)
template <typename T, typename Comparator>
class Heap{
private:
    Comparator cmp;
    std::vector<T> array;

    int sift_down(int i) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int tiniest = i;
        if (left < array.size() && cmp(array[left],  array[i])) {
            tiniest = left;
        }
        if (right < array.size() && cmp(array[right], array[tiniest])) {
            tiniest = right;
        }
        if (tiniest != i) {
            std::swap(array[i], array[tiniest]);
            sift_down(tiniest);
        }
        return 0;
    }

    int sift_up(int i) {
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (cmp(array[parent], array[i])) {
                return 0;
            }
            std::swap(array[i], array[parent]);
            i = parent;
        }
        return 0;
    }

public:
    Heap() = default;
    Heap(const Heap &other) = delete;
    Heap& operator=(const Heap &other) = delete;

    int insert(T value) {
        array.push_back(value);
        sift_up(array.size() - 1);
        return 0;
    }
    T extract_top() {
        T result = array[0];
        array[0] = array.back();
        array.pop_back();
        if (!array.empty()) {
            sift_down(0);
        }
        return result;
    }

    bool is_empty() {
        return array.empty();
    }

    bool more_then_one_left() {
        return array.size() > 1;
    }
};

/// Нода Хаффмана (атрибут leaf используется только при декодировании, и был добавлен позже, поэтому никак не обрабатывается при кодировании)
struct HuffmanNode {
    HuffmanNode(const byte &value, size_t freq) : val(value), freq(freq), right(nullptr), left(nullptr), leaf(false) {};
    explicit HuffmanNode(const byte &value) : val(value), freq(0), right(nullptr), left(nullptr), leaf(false) {};
    byte val;
    size_t freq;
    bool leaf;
    HuffmanNode *right;
    HuffmanNode *left;
};

/// Функтор сравнения Нод для кучи.
class HuffmanNodeComparator {
public:
    bool operator ()(const HuffmanNode *l, const HuffmanNode *r){
        if (l->freq < r->freq) {
            return true;
        }
        return false;
    }
};

/// Функтор, чтобы красиво выводить Ноду при отладке
class PrintHuffmanNode {
public:
    void operator ()(const HuffmanNode *node) {
        if (node->val == 0) {
            std::cout << "val 0" << " freq " << node->freq << " ";
            return;
        }
        std::cout << "val " << (char)node->val << " freq " << node->freq << " ";
    }
};

/// Записывает дерево в выходной поток
void WriteTree(const HuffmanNode *root, BitWriter &out, int &written_all) {
    if (not root->left and not root->right) {
        out.WriteBit(1);
        out.WriteByte(root->val);
        written_all--;
        return;
    }
    WriteTree(root->left, out, written_all);
    WriteTree(root->right, out, written_all);
    if (written_all)
        out.WriteBit(0);
}

/// Функция, заполняющая таблицу соответствия символа с его кодом. Возвращает общую длину итоговой последовательности в битах
int FillHuffmanTable(const HuffmanNode *node, std::vector<bool>& path, std::unordered_map<byte, std::vector<bool>>& table) {
    int count = 0;
    if (not node->left and not node->right) {
        table[node->val] = path;
        count += static_cast<int>(path.size() * node->freq);
    }

    if (node->left) {
        path.push_back(false);
        count += FillHuffmanTable(node->left, path, table);
        path.pop_back();
    }

    if (node->right) {
        path.push_back(true);
        count += FillHuffmanTable(node->right, path, table);
        path.pop_back();
    }
    return count;
}

/// Функция, с помощью которой можно посмотреть на структуру дерева Хаффмана
void printLayers(HuffmanNode* root) {
    PrintHuffmanNode prt;

    if (root == nullptr) {
        return;
    }

    std::queue<std::pair<HuffmanNode*, HuffmanNode*>> nodes;
    nodes.emplace(root, nullptr);

    while (!nodes.empty()) {
        int levelSize = static_cast<int>(nodes.size());

        for (int i = 0; i < levelSize; ++i) {
            HuffmanNode* node = nodes.front().first;
            HuffmanNode* parent = nodes.front().second;
            nodes.pop();

            if (parent != nullptr) {
                prt(node);
                std::cout << " parent ";
                prt(parent);
                std::cout << std::endl;
            } else {
                prt(node);
                std::cout << " parent none\n";
            }

            if (node->left != nullptr) {
                nodes.emplace(node->left, node);
            }
            if (node->right != nullptr) {
                nodes.emplace(node->right, node);
            }
        }

        std::cout << '\n';
    }
}

/// Функция, которая вычисляет длины кодов для каждого символа и кладёт в словарь code_lengths
void FillCodeLengthsTable(HuffmanNode* node, int depth, std::unordered_map<byte, int>& code_lengths) {
    if (not node->right and not node->left) {
        code_lengths[node->val] = depth;
    } else {
        FillCodeLengthsTable(node->left, depth + 1, code_lengths);
        FillCodeLengthsTable(node->right, depth + 1, code_lengths);
    }
}

/// Функция, которая на основании длин кода каждого символа генерирует эти коды таким образом, чтобы меньшему символу (по алфавиту)
/// соответствовал меньший по значению код, если у этих символов длины кодов одинаковые
void CreateCanonicalCodeTable(const std::unordered_map<byte, int>& code_lengths, std::unordered_map<byte, std::vector<bool>>& code_table) {
    std::vector<std::pair<byte, int>> sorted_symbols(code_lengths.begin(), code_lengths.end());
    std::sort(sorted_symbols.begin(), sorted_symbols.end(),  [](const auto& a, const auto& b) {
        return a.second < b.second || (a.second == b.second && a.first < b.first);
    });


    std::vector<bool> code;
    for (const auto& symbol : sorted_symbols) {
        while (code.size() < symbol.second) {
            code.push_back(false);
        }
        code_table[symbol.first] = code;

        for (auto && it : std::ranges::reverse_view(code)) {
            if (!it) {
                it = true;
                break;
            } else {
                it = false;
            }
        }
    }
}

/// Функция, которая получает длины кодов для каждого символа и заносит их в вектор длины 256, таким образом, что если
/// символ встречается (т.е. его длина > 0) то записывается длина, а если нет -- то записывается 0.
/// Итоговый вид вектора [0, 0, 3, 0, 0, 0, 2, 6, ...]
void WriteLengthList(std::vector<byte> &lengths,const std::unordered_map<byte, int> &code_lengths) {
    assert(lengths.size() == 256);
    for (int i = 0; i < 256; ++i) {
        if (code_lengths.find(i) != code_lengths.end()) {
            lengths[i] = static_cast<byte>(code_lengths.find(i)->second);
        } else {
            lengths[i] = 0;
        }
    }
}

/// Записывает полученный (в предыдущей функции) вектор побайтово в выходной поток
void WriteCanonicalTable(const std::unordered_map<byte, int>& code_lengths, BitWriter& out) {
    std::vector<byte> lengths(256, 0);

    WriteLengthList(lengths, code_lengths);

    for (int i = 0; i < 256; ++i) {
        out.WriteFiveBits(lengths[i]);
    }
}

/// Функция, размещающая ноду на нужное место относительно родительской вершины. Используется при восстановлении дерева Хаффмана
/// из словаря длин кодов (при декодировании)
bool InsertNewHuffmanNode(HuffmanNode* node, int length, byte symbol) {
    if (node->leaf) {
        return false;
    }
    if (length == 0) {
        node->val = symbol;
        node->leaf = true;
        return true;
    }

    if (not node->left) {
        auto *new_node = new HuffmanNode(0);
        node->left = new_node;
    }

    if (InsertNewHuffmanNode(node->left, length - 1, symbol)) {
        return true;
    }

    if (not node->right) {
        auto *new_node = new HuffmanNode(0);
        node->right = new_node;
    }
    return InsertNewHuffmanNode(node->right, length - 1, symbol);
}

/// Полностью восстанавливает дерево Хаффмана при декодировании из словаря длин кодов.
HuffmanNode* CreateCanonicalTree(const std::unordered_map<unsigned char, int>& code_lengths) {
    std::vector<std::pair<byte, int>> sorted_symbols(code_lengths.begin(), code_lengths.end());
    std::sort(sorted_symbols.begin(), sorted_symbols.end(),  [](const auto& a, const auto& b) {
        return a.second < b.second || (a.second == b.second && a.first < b.first);
    });

    auto *root = new HuffmanNode(0);

    for (auto p : sorted_symbols) {
        InsertNewHuffmanNode(root, p.second, p.first);
    }

    return root;
}


/// Читает из входного потока в формате, которы записывала функция WriteCanonicalTable(). Т.е. [0, 0, 3, 4, 0, 1, ...]
/// и возвращает указатель на корневую вершину восстановленного дерева
HuffmanNode* ReadCanonicalTable(BitReader& in) {
    std::unordered_map<byte, int> code_lengths;
    for (int i = 0; i < 256; ++i) {
        byte length = in.ReadFiveBits();
        if (length > 0) {
            code_lengths[i] = length;
        }
    }

    HuffmanNode* root = CreateCanonicalTree(code_lengths);

    return root;
}

//HuffmanNode* BuildHuffmanTree(const std::vector<byte> &input) {
//    std::unordered_map<byte, size_t> count_map;
//    // Проходим по входной последовательности и заносим в словарь частотность повторения символов
//    for (auto symbol : input) {
//        count_map[symbol]++;
//    }
//
//    Heap<HuffmanNode*, HuffmanNodeComparator> heap;
//
//    byte alphabet_count = 0;
//
//    // Заносим все символы и их частотность в кучу, попутно считаем размерность алфавита
//    for (const auto &pair : count_map) {
//        auto *node = new HuffmanNode(pair.first, pair.second);
//        heap.insert(node);
//        alphabet_count++;
//    }
//    // Строим дерево Хаффмана
//    while (heap.more_then_one_left()) {
//        auto *node = new HuffmanNode(0);
//        HuffmanNode *left = heap.extract_top();
//        HuffmanNode *right = heap.extract_top();
//        node->freq = left->freq + right->freq;
//        node->left = left;
//        node->right = right;
//        heap.insert(node);
//    }
//
//    HuffmanNode *root = heap.extract_top();
//    return root;
//}
/// Функция, которая удаляет дерево Хаффмана, чтобы освободить память
void deleteTree(HuffmanNode* root) {
    if(root != nullptr) {
        deleteTree(root->left);
        deleteTree(root->right);
        delete root;
        root = nullptr;
    }
}

/// Функция кодирования
void Encode(IInputStream& original, IOutputStream& compressed) {

    std::vector<byte> orig_vect;
    std::unordered_map<byte, size_t> count_map;
    byte value;
    // Проходим по входной последовательности и заносим в словарь частотность повторения символов
    while (original.Read(value)) {
        orig_vect.push_back(value);
        count_map[value]++;
    }

    Heap<HuffmanNode*, HuffmanNodeComparator> heap;

    byte alphabet_count = 0;

    // Заносим все символы и их частотность в кучу, попутно считаем размерность алфавита
    for (const auto &pair : count_map) {
        auto *node = new HuffmanNode(pair.first, pair.second);
        heap.insert(node);
        alphabet_count++;
    }
    // Строим дерево Хаффмана
    while (heap.more_then_one_left()) {
        auto *node = new HuffmanNode(0);
        HuffmanNode *left = heap.extract_top();
        HuffmanNode *right = heap.extract_top();
        node->freq = left->freq + right->freq;
        node->left = left;
        node->right = right;
        heap.insert(node);
    }

    HuffmanNode *root = heap.extract_top();

    BitWriter out;
    if (alphabet_count >= 128) {
        out.WriteBit(1); // Сжатие
        out.WriteBit(1); // "Табличный" режим

        std::unordered_map<byte, int> code_lengths;
        FillCodeLengthsTable(root, 0, code_lengths);

        // Создание канонической таблицы кодов
        std::unordered_map<byte, std::vector<bool>> code_table;
        CreateCanonicalCodeTable(code_lengths, code_table);

        WriteCanonicalTable(code_lengths, out);

        int count_symbols = 0;
        for (const auto& k : code_lengths) {
            count_symbols += static_cast<byte>(count_map[k.first] * k.second);
        }

        out.WriteThreeBits(static_cast<byte>((count_symbols + out.GetBitCount() + 3) % 8));

        for (byte i : orig_vect) {
            for (auto && j : code_table[i]) {
                out.WriteBit(j);
            }
        }
    } else {
        out.WriteBit(1);  // Сжатие
        out.WriteBit(0);  // Режим "дерева"
        // Записываем количество "букв" в алфавите
        out.WriteByte(alphabet_count);
        // Записываем дерево
        int written_all = alphabet_count;
        WriteTree(root, out, written_all);

        // Получаем последовательности для кодирования символов
        std::unordered_map<byte, std::vector<bool>> code_table;
        std::vector<bool> path;

        // Получаем длину закодированной последовательности и записываем сколько бит в последнем байте (число от 0 до 7, поэтому пишем в 3 бита)
        int count_symbols = FillHuffmanTable(root, path, code_table); // Передаём по ссылке -- изменяется внутри функции
        out.WriteThreeBits(static_cast<byte>((count_symbols + out.GetBitCount() + 3) % 8));

        // Записываем входную последовательность её кодами
        for (byte i: orig_vect) {
            for (auto &&j: code_table[i]) {
                out.WriteBit(j);
            }
        }
    }
    deleteTree(root);

    if (out.GetBuffer().size() > orig_vect.size()) {
//        std::cout << out.GetBuffer().size() << " > " << orig_vect.size() << std::endl;
        BitWriter copypast;
        copypast.WriteBit(0);  // Без сжатия
        for (byte symbol : orig_vect) {
            copypast.WriteByte(symbol);
        }
        for (byte k : copypast.GetBuffer()) {
            compressed.Write(k);
        }
    } else {
        // Полностью записываем весь буффер в вывод
        for (byte k: out.GetBuffer()) {
            compressed.Write(k);
        }
    }
}

/// Функция восстанавливает дерево Хаффмана из ввода и возвращает вершину. Делает это сразу в формате 1"символ"1"символ"0...
HuffmanNode* GetHuffmanTree(BitReader &input) {
    byte alphabet_count = input.ReadByte();
    std::stack<HuffmanNode *> stack;

    byte current_bit = 0;
    HuffmanNode *root = nullptr;

    while (alphabet_count) {
        current_bit = input.ReadBit();

        if (current_bit == 1) {
            byte symbol = input.ReadByte();
            auto *node = new HuffmanNode(symbol);
            stack.push(node);
            alphabet_count--;
        } else if (current_bit == 0) {
            HuffmanNode *right = stack.top();
            stack.pop();
            HuffmanNode *left = stack.top();
            stack.pop();

            auto new_node = new HuffmanNode(0);
            new_node->right = right;
            new_node->left = left;
            stack.push(new_node);
        }
    }

    while (stack.size() > 1) {
        HuffmanNode *right = stack.top();
        stack.pop();
        HuffmanNode *left = stack.top();
        stack.pop();

        auto new_node = new HuffmanNode(0);
        new_node->right = right;
        new_node->left = left;
        stack.push(new_node);
    }

    root = stack.top();
    stack.pop();
    return root;
}

/// Декодирует входной поток с помощью дерева Хаффмана, вершину которого принимает и выводит посимвольно в выходной вектор
void DecodeWithTree(HuffmanNode *root, BitReader &input, std::vector<byte> &out) {
    byte last_bit = input.ReadThreeBits();
    byte current_bit = 0;
    HuffmanNode *node = root;
    // Пока не считали последний бит расшифровываем входную последовательность с помощью дерева Хаффмана
    while (not input.gotLast(last_bit)) {
        current_bit = input.ReadBit();
        if (current_bit == 0) {
            node = node->left;
        } else if (current_bit == 1) {
            node = node->right;
        } else {
            assert(current_bit == 0 or current_bit == 1);
        }

        if (not node->left and not node->right) {
            out.push_back(node->val);
            node = root;
        }
    }
}

/// Метод восстанавливает оригинальные данные
void Decode(IInputStream& compressed, IOutputStream& original) {
    BitReader input;
    // Считываем вход сразу в буффер
    input.FillBuffer(compressed);

    byte current_bit = 0;
    HuffmanNode* root = nullptr;
    byte mode = input.ReadBit();
    if (mode == 0) {
        while (not input.gotLast(1)) {
            byte symbol = input.ReadByte();
            original.Write(symbol);
        }
    } else {
        mode = input.ReadBit();


        // Определяем формат кодировки (таблица длин/непосредственно дерево)
        if (mode == 1) {
            // Таблица
            root = ReadCanonicalTable(input);
        } else {
            // Дерево
            root = GetHuffmanTree(input);
        }
        std::vector<byte> output;
        DecodeWithTree(root, input, output);

        // Выводим полученную последовательность декодированных символов в выходной поток
        for (byte k: output) {
            original.Write(k);
        }
        deleteTree(root);
    }
}

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");

    {
        FileInputStream inputStream("input_encode.txt");
        FileOutputStream outputStream("output_encode.txt");
        Encode(inputStream, outputStream);
    }

    {
        FileInputStream dec("output_encode.txt");
        FileOutputStream ou("output_decode.txt");

        Decode(dec, ou);
    }
    return 0;
}