#include <iostream>
#include <memory>
#include <optional>

template <typename T>
class DoublyLinkedList {
private:
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
    class iterator {
    private:
        std::shared_ptr<Node> current;

    public:
        iterator(std::shared_ptr<Node> node) : current(std::move(node)) {}

        T& operator*() {
            return current->value;
        }

        iterator operator++(int) {
            iterator tmp = *this;
            if (current) {
                current = current->next;
            }
            return tmp;
        }

        bool operator==(const iterator& other) const {
            return current == other.current;
        }

        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }
    };

    DoublyLinkedList() : head(nullptr), length(0) {}

    bool empty() const {
        return length == 0;
    }

    size_t size() const {
        return length;
    }

    void push_front(const T& value) {
        auto new_node = std::make_shared<Node>(value);
        new_node->next = head;
        if (head) {
            head->prev = new_node;
        } else {
            tail = new_node;
        }
        head = std::move(new_node);
        ++length;
    }

    void push_back(const T& value) {
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

    std::optional<T> pop_front() {
        if (!head) {
            return std::nullopt;
        }
        T ret = head->value;
        auto new_head = head->next;
        if (new_head) {
            new_head->prev.reset();
        } else {
            tail.reset();
        }
        head = std::move(new_head);
        --length;
        return ret;
    }

    std::optional<T> pop_back() {
        if (empty()) {
            return std::nullopt;
        }
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

    iterator begin() { return iterator(head); }
    iterator end() { return iterator(nullptr); }
};
