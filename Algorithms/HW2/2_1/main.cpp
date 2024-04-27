// Дано число N ≤ 104 и последовательность целых чисел из [-231..231] длиной N.
// Требуется построить бинарное дерево, заданное наивным порядком вставки.
// Т.е., при добавлении очередного числа K в дерево с корнем root,
// если root→Key ≤ K, то узел K добавляется в правое поддерево root; иначе в левое поддерево root.
// Выведите элементы в порядке in-order (слева направо).
//
// Рекурсия запрещена.




// Для тестирования реализовал функции удаления и проверки существования элемента (в рамках задачи не используются).
#include <iostream>
#include <stack>

template <typename T, typename Comparator, typename Equiv>
class BinTree {
private:
    struct Node {
        explicit Node(const T value) : data(value), right(nullptr), left(nullptr), parent(nullptr) {};
        T data;
        Node *right;
        Node *left;
        Node *parent;
    };

    bool del_with_zero_or_one_child(Node *current) {

        Node *parent_t = current->parent;
        if (current->left and current->right) {
            return false;
        }
        Node *child = current->left ? current->left : current->right;

        if (parent_t) {
            if (parent_t->right == current) {
                parent_t->right = child;
            } else {
                parent_t->left = child;
            }
        } else {
            root = child;
        }

        if (child) {
            child->parent = parent_t;
        }

        delete current;
        return true;
    }


    bool del_with_two_children(Node *current) {
        Node *min_right = current->right;
        while (min_right->left) {
            min_right = min_right->left;
        }
        current->data = min_right->data;
        del_with_zero_or_one_child(min_right);
        return true;
    }

    Node* Find(const T &value) {
        Node *current = root;
        while (current) {
            if (equ(value, current->data)) {
                return current;
            } else if (cmp(value, current->data)) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
        return nullptr;
    }

    Node *root;
    Comparator cmp;
    Equiv equ;
public:
    BinTree() : root(nullptr) {};

    BinTree(const BinTree&) = delete;
    BinTree& operator=(const BinTree&) = delete;

    ~BinTree() {
        if (not root) {
            return;
        }
        std::stack<Node*> s1, s2;
        s1.push(root);
        Node* node;

        while (!s1.empty()) {
            node = s1.top();
            s1.pop();
            s2.push(node);

            if (node->left) {
                s1.push(node->left);
            }
            if (node->right) {
                s1.push(node->right);
            }
        }

        while (!s2.empty()) {
            node = s2.top();
            s2.pop();
            delete node;
        }

    }

    bool Add(const T &value) {
        Node *new_elem = new Node(value);
        if (not root) {
            root = new_elem;
            return true;
        }
        Node *current = root;
        while (current) {
            if (cmp(new_elem->data, current->data)) {
                if (current->left) {
                    current = current->left;
                } else {
                    current->left = new_elem;
                    new_elem->parent = current;
                    return true;
                }
            } else {
                if (current->right) {
                    current = current->right;
                } else {
                    current->right = new_elem;
                    new_elem->parent = current;
                    return true;
                }
            }
        }
        return false;
    }

    bool Delete(const T &value) {
        Node *current = Find(value);
        std::cout << current->data << std::endl;
        if (not current) {
            return false;
        }
        if (not equ(current->data, value)) {
            return false;
        }

        if (del_with_zero_or_one_child(current)) {
            return true;
        } else {
            del_with_two_children(current);
            return true;
        }
    }


    bool Has(const T &value) {
        if (Find(value)) {
            return true;
        }
        return false;
    }

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
    }
};

class IntComparator {
public:
    bool operator()(int a, int b) const {
        return a < b;
    }
};

class IntEquivalence {
public:
    bool operator()(int a, int b) const {
        return a == b;
    }
};


//void process() {
//
//    BinTree<int, IntComparator, IntEquivalence> tree;
//    char op;
//    int key;
//
//    while (std::cin >> op >> key)
//    {
//        switch (op)
//        {
//            case '?':
//            {
//                std::cout << (tree.Has(key) ? "OK" : "FAIL") << std::endl;
//                break;
//            }
//            case '+':
//            {
//                std::cout << (tree.Add(key) ? "OK" : "FAIL") << std::endl;
//                break;
//            }
//            case '-':
//            {
//                std::cout << (tree.Delete(key) ? "OK" : "FAIL") << std::endl;
//                break;
//            }
//            case '!':
//            {
//                tree.inOrder();
//                break;
//            }
//            default:
//            {
//                return;
//            }
//        }
//    }
//}
void process() {

    BinTree<int, IntComparator, IntEquivalence> tree;
    int n = 0;
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        int value = 0;
        std::cin >> value;
        tree.Add(value);
    }
    tree.inOrder();
}


int main() {
    process();
    return 0;
}
