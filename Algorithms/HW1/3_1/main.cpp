// Во всех задачах из следующего списка следует написать структуру данных, обрабатывающую команды push* и pop*.
// Формат входных данных.
// В первой строке количество команд n. n ≤ 1000000.
// Каждая команда задаётся как 2 целых числа: a b.
// a = 1 - push front
//        a = 2 - pop front
//        a = 3 - push back
//        a = 4 - pop back
//        Команды добавления элемента 1 и 3 заданы с неотрицательным параметром b.
// Для очереди используются команды 2 и 3. Для дека используются все четыре команды.
// Если дана команда pop*, то число b - ожидаемое значение. Если команда pop вызвана для пустой структуры данных, то ожидается “-1”.
// Формат выходных данных.
// Требуется напечатать YES - если все ожидаемые значения совпали. Иначе, если хотя бы одно ожидание не оправдалось, то напечатать NO.
//
// 3_1. Реализовать очередь с динамическим зацикленным буфером (на основе динамического массива).
// Требования: Очередь должна быть реализована в виде класса.

#include <iostream>

#define INIT_SIZE 5

class Queue {
private:
    int head = 0;
    int tail = 0;
    int size = 0;
    int *array = nullptr;
    int count_values = 0;
public:
    Queue() {
        size = INIT_SIZE;
        array = new int[INIT_SIZE];
    }
    Queue(const Queue &other) = delete;
    Queue& operator=(const Queue &other) = delete;

    ~Queue() {
        delete[] array;
    }
    [[nodiscard]] bool is_empty() const {
        if (count_values) {
            return false;
        }
        return true;
    }
    int push_back (int value) {
        if (not is_empty() and tail == head) {
            extend();
        }
        array[tail] = value;
        tail = (++tail) % size;
        count_values++;
        return 0;
    }
    int extend() {
        int *old_array = array;
        array = new int[size * 2];
        int index_old = head;
        int index_new = 0;

        while (index_new < size) {
            array[index_new++] = old_array[(index_old++) % size];
        }
        head = 0;
        tail = size;
        size *= 2;
        return 0;
    }
    int pop_front() {
        if (is_empty()) {
            return -1;
        }
        count_values--;
        int value = array[head++];
        head = head % size;
        return value;
    }

};

bool correct_response(Queue *queue, int command, int value) {
    if (command == 3) {
        queue->push_back(value);
        return true;
    }
    if (command == 2) {
        int given_value = queue->pop_front();
        if (value == given_value) {
            return true;
        }
    }
    return false;
}

int main() {
    int n, command, value;
    n = command = value = 0;
    std::cin >> n;
    Queue queue = Queue();
    bool flag = true;
    for (int i = 0; i < n; i++) {
        std::cin >> command;
        std::cin >> value;
        flag = correct_response(&queue, command, value);
    }
    if (flag) {
        std::cout << "YES" << std::endl;
    } else {
        std::cout << "NO" << std::endl;
    }
    return 0;
}
