// Постройте B-дерево минимального порядка t и выведите его по слоям.
// В качестве ключа используются числа, лежащие в диапазоне [0..2^32-1]
// Требования:
//
// B-дерево должно быть реализовано в виде шаблонного класса.
// Решение должно поддерживать передачу функции сравнения снаружи.
// Формат ввода
// Сначала вводится минимальный порядок дерева t.
// Затем вводятся элементы дерева.
//
// Формат вывода
// Программа должна вывести B-дерево по слоям.
// Каждый слой на новой строке, элементы должны выводится в том порядке, в котором они лежат в узлах.


#include <iostream>
#include <vector>
#include <queue>
#include <sstream>

template <typename T, typename Comparator>
class BTree {
public:
    struct Node {
        Node(bool leaf) : leaf(leaf) {}

        ~Node() {
            for (Node* child: children) {
                delete child;
            }
        }

        bool leaf;
        std::vector<T> keys;
        std::vector<Node*> children;
    };

    BTree(size_t min_degree) : t(min_degree), root(nullptr) {
    }

    ~BTree() {
        if (root)
            delete root;
    }

    void Insert(const T &key) {
        if (!root)
            root = new Node(true);

        // здесь и дальше: если при спуске по дереву находим
        // переполненный узел -- сначала разбиваем его, потом спускаемся
        if (isNodeFull(root)) {
            Node *newRoot = new Node(false);
            newRoot->children.push_back(root);
            root = newRoot;
            splitChild(root, 0);
        }

        // теперь корень точно не переполнен, можем вызвать insertNonFull
        insertNonFull(root, key);
    }

    void debugPrint() {
        if (root)
            debugPrintInternal(root, 0);
    }

    void BFS() {
        if (root == nullptr)
            return;

        std::queue<Node*> q;
        std::queue<int> levels;
        q.push(root);
        levels.push(0);
        size_t current_level = 0;

        while (not q.empty()) {
            Node* temp = q.front();
            int level = levels.front();
            q.pop();
            levels.pop();

            if (level > current_level) {
                std::cout << std::endl;
                current_level = level;
            }

            for (size_t i = 0; i < temp->keys.size(); ++i) {
                std::cout << temp->keys[i] << " ";
            }

            if (not temp->leaf) {
                for (size_t j = 0; j < temp->children.size(); ++j) {
                    q.push(temp->children[j]);
                    levels.push(level + 1);
                }
            }
        }
        std::cout << std::endl;
    }

private:

    void debugPrintInternal(Node *node, int indent) {
        std::cout << std::string(indent, ' ');
        std::cout << "keys: [";
        for (auto it = node->keys.begin(); it != node->keys.end(); it++) {
            std::cout << (*it);
            if (it + 1 != node->keys.end())
                std::cout << ", ";
        }
        std::cout << "]" << std::endl;

        for (auto child: node->children) {
            debugPrintInternal(child, indent + 4);
        }
    }

    bool isNodeFull(Node *node) {
        return node->keys.size() == 2 * t - 1;
    }

    // разбить переполненного потомка index узла node
    void splitChild(Node *node, size_t index) {
        Node *Y = node->children[index];
        Node *Z = new Node(Y->leaf);
        size_t half = t - 1;
        for (size_t j = 0; j < half; ++j) {
            Z->keys.push_back(Y->keys[j+t]);
        }

        // Убираем элементы, перенесённые в Z из Y
        for (size_t j = 0; j < half; ++j)
            Y->keys.pop_back();

        if (not Y->leaf) {
            for (size_t j = 0; j < t; ++j) {
                Z->children.push_back(Y->children[j+t]);
            }

            // Убираем перенесённых потомков
            for (size_t j = 0; j < t; ++j) {
                Y->children.pop_back();
            }
        }

        node->children.push_back(nullptr); // Расширяем вектор потомков (для вставки ещё одного)
        for (size_t j = node->children.size() - 1; j > index; --j) {
            node->children[j] = node->children[j-1];
        }

        node->children[index+1] = Z;

        // Добавляем в конец последний элемент, чтобы расширить вектор
        T empty_key = T();
        node->keys.push_back(empty_key);

        if (node->keys.size() != 1) {
            for (size_t j = node->keys.size() - 1; j > index; --j) {
                node->keys[j] = node->keys[j - 1];
            }
        }

        node->keys[index] = Y->keys[t-1];
        Y->keys.pop_back(); // Убираем медианный элемент из Y
    }

    // вставить ключ key в гарантированно не переполненную ноду node
    void insertNonFull(Node *node, const T &key) {
        int pos = node->keys.size() - 1;

        // гарантированно не перепеполненный лист -- запишем новый ключ в него
        if (node->leaf) {
            // расширили вектор ключей для вставки нового
            node->keys.resize(node->keys.size() + 1);

            while (pos >= 0 && cmp(key, node->keys[pos])) {
                // обходим ключи справа налево, сдвигая вправо на 1
                node->keys[pos + 1] = node->keys[pos];
                pos--;
            }
            // вставляем новый ключ на освобожденное в цикле место
            node->keys[pos + 1] = key;
        }
            // не лист, значит есть потомки, пишем в один из них
        else {
            // ищем позицию потомка, в которого добавим ключ
            while (pos >= 0 && cmp(key, node->keys[pos])) {
                pos--;
            }

            // если потомок и так полон, надо его сначала разбить
            if (isNodeFull(node->children[pos + 1])) {
                splitChild(node, pos + 1);
                // после разбиения потомка в текущий узел из него поднялся ключ
                // надо сравниться и с ним

                if (cmp(node->keys[pos + 1], key))
                    pos++;
            }
            insertNonFull(node->children[pos + 1], key);
        }
    }

    size_t t;
    Node *root;
    Comparator cmp;

    friend void test1();
};

class MyComparator {
public:
    bool operator()(int a, int b) const {
        return a < b;
    }
};

// случаи вставки с иллюстраций в лекции
void test1()
{
    BTree<char, MyComparator> tree(3);

    tree.root = new BTree<char, MyComparator>::Node(false);
    tree.root->keys = {'G', 'M', 'P', 'X'};

    {
        auto child = new BTree<char, MyComparator>::Node(true);
        child->keys = {'A', 'C', 'D', 'E'};
        tree.root->children.push_back(child);
    }

    {
        auto child = new BTree<char, MyComparator>::Node(true);
        child->keys = {'J', 'K'};
        tree.root->children.push_back(child);
    }
    {
        auto child = new BTree<char, MyComparator>::Node(true);
        child->keys = {'N', 'O'};
        tree.root->children.push_back(child);
    }
    {
        auto child = new BTree<char, MyComparator>::Node(true);
        child->keys = {'R', 'S', 'T', 'U', 'V'};
        tree.root->children.push_back(child);
    }
    {
        auto child = new BTree<char, MyComparator>::Node(true);
        child->keys = {'Y', 'Z'};
        tree.root->children.push_back(child);
    }

    std::cout << "Initial tree:" << std::endl;
    tree.debugPrint();
    std::cout << std::endl;

    std::string insertKeys = "BQLF";
    // посимвольно добавляем в дерево ключи
    for (auto c: insertKeys) {
        tree.Insert(c);
        std::cout << "After inserting " << c << ":" << std::endl;
        tree.BFS();
        tree.debugPrint();
        std::cout << std::endl;
    }
}

void process() {
    size_t t = 0;
    std::cin >> t;

    BTree<size_t, MyComparator> tree(t);
    size_t number = 0;

    std::string line;
    std::getline(std::cin, line);
    std::getline(std::cin, line);

    std::istringstream iss(line);

    while (iss >> number) {
        tree.Insert(number);
    }
    tree.BFS();
}

int main(int argc, const char * argv[]) {
    process();
    return 0;
}