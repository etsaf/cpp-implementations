//simplified std::list implementation

#include <iostream>
#include <utility>

template <typename T>
class List {
private:
    struct Node {
        T value;
        Node* next;
        Node* prev;
        Node(const T& n)
                : value(n), next(), prev() {}
    };

    int sz = 0;
    Node* head = nullptr;
    Node* tail = nullptr;

    void deallocate() {
        while (head) {
            tail = head->next;
            delete head;
            head = tail;
        }
    }

public:
    List() {}

    List(const List& other) {
        auto curr = other.head;
        while (curr != nullptr) {
            push_back(curr->value);
            curr = curr->next;
        }
    }

    List& operator=(const List& other) {
        if (this == &other) {
            return *this;
        }
        deallocate();
        auto curr = other.head;
        while (curr != nullptr) {
            push_back(curr->value);
            curr = curr->next;
        }
        return *this;
    }

    size_t size() const {
        return sz;
    }

    void push_back(const T& elem) {
        Node* tmp = new Node(elem);
        if (sz == 0) {
            head = tmp;
        } else {
            tail->next = tmp;
            tmp->prev = tail;
        }
        tail = tmp;
        ++sz;
    }

    void pop_back() {
        if (sz == 1) {
            delete tail;
            head = tail = nullptr;
        } else {
            auto tmp = tail->prev;
            delete tail;
            tail = tmp;
            tail->next = nullptr;
        }
        --sz;
    }

    void push_front(const T& elem) {
        Node* tmp = new Node(elem);
        if (sz == 0) {
            tail = tmp;
        } else {
            head->prev = tmp;
            tmp->next = head;
        }
        head = tmp;
        ++sz;
    }

    void pop_front() {
        if (sz == 1) {
            delete head;
            head = tail = nullptr;
        } else {
            auto tmp = head->next;
            delete head;
            head = tmp;
            head->prev = nullptr;
        }
        --sz;
    }

    ~List() {
        deallocate();
    }

    class Iterator {
    private:
        Node* It;
        const List* Base;

    public:
        Iterator(Node* it, const List* base)
        : It(it), Base(base) {}

        const T& operator*() const {
            return It->value;
        }

        Iterator operator++() {
            It = It->next;
            return *this;
        }

        Iterator operator--() {
            if (It == nullptr) {
                It = Base->tail;
            } else {
                It = It->prev;
            }
            return *this;
        }

        bool operator==(const Iterator& other) {
            return It == other.It;
        }

        bool operator!=(const Iterator& other) {
            return !(*this == other);
        }
    };

    const Iterator begin() const {
        return Iterator(head, this);
    }

    const Iterator end() const {
        return Iterator(tail->next, this);
    }
};