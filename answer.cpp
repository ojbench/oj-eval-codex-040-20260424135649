#include <iostream>
#include <memory>
#include <optional>

template <typename T>
class DoublyLinkedList {
private:
    // Node structure definition
    struct Node {
        T value;
        std::shared_ptr<Node> next;
        std::weak_ptr<Node> prev;

        Node(const T& val) : value(val), next(nullptr) {}
    };

    std::shared_ptr<Node> head;
    std::weak_ptr<Node> tail;
    size_t length;

public:
    // Iterator class
    class iterator {
    private:
        std::shared_ptr<Node> current;

    public:
        iterator(std::shared_ptr<Node> node) : current(node) {}

        // Dereference operator to get value
        T& operator*() {
            // 注：对空迭代器调用此函数是未定义行为，测试数据中不会出现
            // TODO(student)
            return current->value;
        }

        // Post-increment operator
        iterator operator++(int) {
            // 注：对空迭代器调用此函数是未定义行为，测试数据中不会出现
            // TODO(student)
            iterator tmp = *this;
            if (current) {
                current = current->next;
            }
            return tmp;
        }

        // Equality operators
        bool operator==(const iterator& other) const {
            // 注：比较属于不同容器的非空迭代器是未定义行为，测试数据中不会出现
            // TODO(student)
            return current == other.current;
        }

        bool operator!=(const iterator& other) const {
            // TODO(student)
            return !(*this == other);
        }
    };

    // Constructor
    DoublyLinkedList() : head(nullptr), length(0) {}

    // No destructor needed, memory managed by shared_ptr

    // Check if list is empty
    bool empty() const {
        return length == 0;
    }

    // Get the size of the list
    size_t size() const {
        return length;
    }

    // Add element to the front
    void push_front(const T& value) {
        // TODO(student)
        auto new_node = std::make_shared<Node>(value);
        new_node->next = head;
        if (head) {
            head->prev = new_node;
        } else {
            tail = new_node;
        }
        head = new_node;
        ++length;
    }

    // Add element to the back
    void push_back(const T& value) {
        // TODO(student)
        auto new_node = std::make_shared<Node>(value);
        auto t = tail.lock();
        if (t) {
            t->next = new_node;
            new_node->prev = t;
            tail = new_node;
        } else {
            head = new_node;
            tail = new_node;
        }
        ++length;
    }

    // Remove and return the first element
    std::optional<T> pop_front() {
        if (!head) {
            return std::nullopt;
        }
        // TODO(student)
        T ret = head->value;
        auto new_head = head->next;
        if (new_head) {
            new_head->prev.reset();
        } else {
            tail.reset();
        }
        head = new_head;
        --length;
        return ret;
    }

    // Remove and return the last element
    std::optional<T> pop_back() {
        if (empty()) {
            return std::nullopt;
        }
        // TODO(student)
        auto t = tail.lock();
        T ret = t->value;
        auto prev = t->prev.lock();
        if (prev) {
            prev->next.reset();
            tail = prev;
        } else {
            head.reset();
            tail.reset();
        }
        --length;
        return ret;
    }

    // Iterators
    iterator begin() {
        return iterator(head);
    }

    iterator end() {
        return iterator(nullptr);
    }
};
