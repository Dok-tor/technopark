//Напишите функцию, проверяющую, что во всех узлах бинарного дерева поиска хранится одинаковое значение.
//Значения - int.
//
//Формат ввода
//По очереди вводятся узлы бинарного дерева в порядке вставки.
//
//Формат вывода
//0 - если все значения в узлах бинарного дерева разные.
//1 - если все значения в узлах бинарного дерева одинаковые.



// Преобразовал функцию inOrder для того, чтобы она во время обхода определяла, все ли значения - одинаковые
#include <iostream>
#include <stack>
#include <sstream>

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

    int inOrder() {
        std::stack<Node*> s;
        Node* current = root;
        if (not current) {
            return 0;
        }
        T value = current->data;

        while (current || !s.empty()) {
            while (current) {
                s.push(current);
                current = current->left;
            }
            current = s.top();
            s.pop();
            if (not equ(current->data, value))
                return 0;

            current = current->right;
        }
        return 1;
    }
    int find() {
        std::cout << findMinDepth(root) << std::endl;
    }

    int findMinDepth(Node* root) {
        if (root == nullptr) {
            return 0;
        }

        if (root->children.empty()) {
            return 1;
        }

        int min_depth = INT_MAX;
        for (Node* child : root->children) {
            int depth = findMinDepth(child);
            min_depth = std::min(min_depth, depth);
        }

        return min_depth + 1;
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



void process() {

    BinTree<int, IntComparator, IntEquivalence> tree;
    std::string line;
    std::getline(std::cin, line);

    std::istringstream iss(line);
    int number;

    while (iss >> number) {
        tree.Add(number);
    }
    tree.find()
}


int main() {
    process();
    return 0;
}
