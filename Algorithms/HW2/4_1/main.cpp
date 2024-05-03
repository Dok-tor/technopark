// В одной военной части решили построить в одну шеренгу по росту.
// Т.к. часть была далеко не образцовая, то солдаты часто приходили не вовремя,
// а то их и вовсе приходилось выгонять из шеренги за плохо начищенные сапоги.
// Однако солдаты в процессе прихода и ухода должны были всегда быть выстроены по росту – сначала самые высокие,
// а в конце – самые низкие. За расстановку солдат отвечал прапорщик,
// который заметил интересную особенность – все солдаты в части разного роста.
// Ваша задача состоит в том, чтобы помочь прапорщику правильно расставлять солдат,
// а именно для каждого приходящего солдата указывать, перед каким солдатом в строе он должен становится.
// Требуемая скорость выполнения команды - O(log n).
//
// Формат ввода
// Первая строка содержит число N – количество команд (1 ≤ N ≤ 90 000).
// В каждой следующей строке содержится описание команды: число 1 и X если солдат приходит в строй (X – рост солдата,
// натуральное число до 100 000 включительно) и число 2 и Y если солдата,
// стоящим в строе на месте Y надо удалить из строя. Солдаты в строе нумеруются с нуля.
//
// Формат вывода
// На каждую команду 1 (добавление в строй) вы должны выводить число K – номер позиции,
// на которую должен встать этот солдат (все стоящие за ним двигаются назад).



#include <iostream>
#include <stack>
#include <queue>

// Внизу определены 2 функции process() для тестирования в с расширенным функционалом (чтение и запись из файла и возможность вывода текущего состояния дерева)
// Для выгрузки на контест я меняю функцию process() на формат контеста.
//#define DEBUG

template <typename T, typename Comparator, typename Equival>
class AvlTree {
    struct Node {
        explicit Node(const T &data) : data(data), left(nullptr), right(nullptr), height(1), count(1) {}

        T data;
        Node *left;
        Node *right;
        size_t height;
        size_t count;
    };

public:
    AvlTree() : root(nullptr) {}

    ~AvlTree() {
        destroyTree(root);
    }

    size_t Add(const T &data) {
        auto [new_root, k] = addInternal(root, data, 0);
        root = new_root;
        return k;
    }

    bool Has(const T &data) {
        Node *tmp = root;
        while (tmp) {
            if (equ(tmp->data, data))
                return true;
            else if (cmp(tmp->data, data))
                tmp = tmp->right;
            else
                tmp = tmp->left;
        }
        return false;
    }
    void Delete(const T &data) {
        root = deleteInternal(root, data);
    }
//    T get_k_data(size_t k) {
//        Node *current = findKInternal(root, k);
//        T data = current->data;
//        return data;
//    }

    void inOrder() {
        std::stack<Node*> s;
        Node* current = root;

        while (current || !s.empty()) {
            while (current) {
                s.push(current);
                current = current->left;
            }
            current = s.top();
            s.pop();

            std::cout << current->data << " ";

            current = current->right;
        }
        std::cout << std::endl;
    }

    void BFS() {
        if (root == nullptr)
            return;

        std::queue<Node*> q;
        q.push(root);

        while (!q.empty()) {
            Node* temp = q.front();
            q.pop();

            std::cout << temp->data << " ";

            if (temp->left != nullptr)
                q.push(temp->left);

            if (temp->right != nullptr)
                q.push(temp->right);
        }
        std::cout << std::endl;
    }
private:
    void destroyTree(Node *node) {
        if (node) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

//    Node* deleteInternal(Node *node, const T &data) {
//        if (!node)
//            return nullptr;
//        if (cmp(node->data, data))
//            node->right = deleteInternal(node->right, data);
//        else if (cmp(data, node->data))
//            node->left = deleteInternal(node->left, data);
//        else {
//            Node *left = node->left;
//            Node *right = node->right;
//
//            delete node;
//
//            if (!right)
//                return left;
//
//            // поддерево, из которого берем элемент взамен удаляемого, выбираем на основе сравнения глубин.
//            // (берем из более глубокого)
//
//            // findMin и removeMin объединить в один метод findAndRemoveMin/findAndRemoveMax
//            Node *min = findMin(right); // возвращает минимальный элемент в дереве
//            min->right = removeMin(right); // возвращает дерево, из которого удалили минимальный элемент
//            min->left = left;
//
//            return doBalance(min);
//        }
//
//        return doBalance(node);
//    }

    Node* deleteInternal(Node *node, size_t k) {
        if (not node)
            return nullptr;
        if (getCount(node->left) > k) {
            node->left = deleteInternal(node->left, k);
        } else if (getCount(node->left) < k) {
            node->right = deleteInternal(node->right, k - getCount(node->left) - 1);
        } else {
            Node *left = node->left;
            Node *right = node->right;

            delete node;

            if (!right)
                return left;

            Node *max_or_min;

            if (getHeight(left) > getHeight(right)) {
                max_or_min = findAndRemoveMax(left);
                max_or_min->right = right;
            } else {
                max_or_min = findAndRemoveMin(right);
                max_or_min->left = left;
            }
            return doBalance(max_or_min);
        }
        return doBalance(node);
    }

    Node* findAndRemoveMin(Node* node) {
        std::stack<Node*> s;
        while (node->left) {
            s.push(node);
            node = node->left;
        }

        if (s.empty()) {
            return node;
        }

        Node *new_root = node;
        node = node->right;

        while (not s.empty()) {
            Node *current = s.top();
            s.pop();
            current->left = node;
            node = doBalance(current);
        }
        new_root->right = node;
        return new_root;
    }

    Node* findAndRemoveMax(Node* node) {
        std::stack<Node*> s;
        while (node->right) {
            s.push(node);
            node = node->right;
        }

        if (s.empty()) {
            return node;
        }

        Node *new_root = node;
        node = node->left;

        while (not s.empty()) {
            Node *current = s.top();
            s.pop();
            current->right = node;
            node = doBalance(current);
        }
        new_root->left = node;
        return new_root;
    }


//    Node* findMin(Node *node) {
//        while (node->left)
//            node = node->left;
//        return node;
//    }
//
//    Node* removeMin(Node *node) {
//        if (!node->left)
//            return node->right;
//        node->left = removeMin(node->left);
//        return doBalance(node);
//    }

    std::pair<Node*, size_t> addInternal(Node *node, const T &data, size_t k) {
        if (not node) {
            return std::make_pair(new Node(data), k);
        }
        Node *result_node;
        size_t k_new = k;
        if (cmp(node->data,  data)) {
            std::tie(result_node, k_new) = addInternal(node->right, data, k + getCount(node->left) + 1);
            node->right = result_node;
        }
        else if (cmp(data,  node->data)){
            std::tie(result_node, k_new) = addInternal(node->left, data, k);
            node->left = result_node;
        }
        return std::make_pair(doBalance(node), k_new);;
    }

    size_t getHeight(Node *node) {
        return node ? node->height : 0;
    }

    size_t getCount(Node *node) {
        return node ? node->count : 0;
    }

    void fixHeight(Node *node) {
        node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
    }

    void fixCount(Node *node) {
        node->count = getCount(node->left) + getCount(node->right) + 1;
    }

    int getBalance(Node *node) {
        return getHeight(node->right) - getHeight(node->left);
    }

//    Node* findKInternal(Node *node, size_t k) {
//        if (not node)
//            return nullptr;
//        if (getCount(node->left) > k) {
//            return findKInternal(node->left, k);
//        } else if (getCount(node->left) == k) {
//            return node;
//        }
//        return findKInternal(node->right, k - getCount(node->left) - 1);
//    }

    Node* rotateLeft(Node *node) {
        Node *tmp = node->right;
        node->right = tmp->left;
        tmp->left = node;
        fixHeight(node);
        fixHeight(tmp);
        fixCount(node);
        fixCount(tmp);
        return tmp;
    }

    Node* rotateRight(Node *node) {
        Node *tmp = node->left;
        node->left = tmp->right;
        tmp->right = node;
        fixHeight(node);
        fixHeight(tmp);
        fixCount(node);
        fixCount(tmp);
        return tmp;
    }

    Node* doBalance(Node *node) {
        fixHeight(node);
        fixCount(node);
        switch (getBalance(node)) {
            case 2: {
                if (getBalance(node->right) < 0)
                    node->right = rotateRight(node->right);
                return rotateLeft(node);
            }
            case -2: {
                if (getBalance(node->left) > 0)
                    node->left = rotateLeft(node->left);
                return rotateRight(node);
            }
            default:
                return node;
        }
    }

private:
    Node *root;
    Comparator cmp;
    Equival equ;
};

class IntComparator {
public:
    bool operator()(int a, int b) const {
        return a > b;
    }
};

class IntEquivalence {
public:
    bool operator()(int a, int b) const {
        return a == b;
    }
};

#ifdef DEBUG
void process() {
    freopen("input.txt","r",stdin);
    freopen("output.txt","w",stdout);

    AvlTree<size_t, IntComparator, IntEquivalence> SoldierAVL;
    char op = '0';
    size_t key = 0;

    while (std::cin >> op >> key)
    {
        switch (op) {
            case '1': {
                std::cout << SoldierAVL.Add(key) << std::endl;
                break;
            }
            case '2': {
                SoldierAVL.Delete(key);
                break;
            }
            case '!': {
                SoldierAVL.inOrder();
                break;
            }
            case '=': {
                SoldierAVL.BFS();
                break;
            }
            default: {
                return;
            }
        }
    }
}
#else
void process() {
    AvlTree<size_t, IntComparator, IntEquivalence> SoldierAVL;
    size_t n = 0;
    std::cin >> n;

    char op = '0';
    size_t key = 0;

    for (size_t i = 0; i < n; ++i) {
        std::cin >> op >> key;
        switch (op) {
            case '1': {
                std::cout << SoldierAVL.Add(key) << std::endl;
                break;
            }
            case '2': {
                SoldierAVL.Delete(key);
                break;
            }
            default:
                break;
        }
    }
}
#endif

int main() {
    process();
    return 0;
}