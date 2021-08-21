//simplified std::vector implementation

#include <iostream>
#include <new>
#include <memory>
#include <utility>

template <typename T>
struct Memory {
    T* buffer = nullptr;
    size_t cp = 0;

    static T* Allocate(size_t n) {
        return static_cast<T*>(operator new(n * sizeof(T)));
    }

    static void Deallocate(T* buffer) {
        operator delete(buffer);
    }

    void swap(Memory& other) noexcept {
        std::swap(buffer, other.buffer);
        std::swap(cp, other.cp);
    }

    Memory() = default;

    Memory(size_t n) {
        buffer = Allocate(n);
        cp = n;
    }

    Memory(const Memory&) = delete;

    Memory(Memory&& other) noexcept {
        swap(other);
    }

    ~Memory() {
        Deallocate(buffer);
    }

    Memory& operator=(const Memory&) = delete;

    Memory& operator=(Memory&& other) noexcept {
        swap(other);
        return *this;
    }

    T* operator+(size_t n) {
        return buffer + n;
    }

    const T* operator+(size_t n) const {
        return buffer + n;
    }

    T& operator[](size_t i) {
        return buffer[i];
    }

    const T& operator[](size_t i) const {
        return buffer[i];
    }
};

template <typename T>
class Vector {
private:
    Memory<T> data;
    size_t sz = 0;

public:
    void swap(Vector& other) noexcept {
        data.swap(other.data);
        std::swap(sz, other.sz);
    }

    Vector() = default;

    Vector(size_t n)
    : data(n) {
        std::uninitialized_value_construct_n(data.buffer, n);
        sz = n;
    }

    Vector(const Vector& other)
    : data(other.sz) {
        std::uninitialized_copy_n(
                other.data.buffer, other.sz, data.buffer);
        sz = other.sz;
    }

    Vector(Vector&& other) noexcept {
        swap(other);
    }

    ~Vector() {
        std::destroy_n(data.buffer, sz);
    }

    Vector& operator=(const Vector& other) {
        if (other.sz > data.cp) {
            Vector tmp(other);
            swap(tmp);
        } else {
            for (size_t i = 0; i < sz && i < other.sz; ++i) {
                data[i] = other.data[i];
            }
            if (sz < other.sz) {
                std::uninitialized_copy_n(
                        other.data.buffer + sz,
                        other.sz - sz,
                        data.buffer);
            } else if (sz > other.sz) {
                std::destroy_n(
                        data.buffer + other.sz,
                        sz - other.sz);
            }
            sz = other.sz;
        }
        return *this;
    }

    Vector& operator=(Vector&& other) noexcept {
        swap(other);
        return *this;
    }

    void reserve(size_t n) {
        if (n > data.cp) {
            Memory<T> data2(n);
            std::uninitialized_move_n(
                    data.buffer, sz, data2.buffer);
            std::destroy_n(data.buffer, sz);
            data = std::move(data2);
        }
    }

    void resize(size_t n) {
        reserve(n);
        if (sz < n) {
            std::uninitialized_value_construct_n(
                    data + sz, n - sz);
        } else if (sz > n) {
            std::destroy_n(data + n, sz - n);
        }
        sz = n;
    }

    void push_back(const T& elem) {
        if (sz == data.cp) {
            if (sz == 0) {
                reserve(1);
            } else {
                reserve(sz * 2);
            }
        }
        new (data + sz) T(elem);
        ++sz;
    }

    void push_back(T&& elem) {
        if (sz == data.cp) {
            if (sz == 0) {
                reserve(1);
            } else {
                reserve(sz * 2);
            }
        }
        new (data + sz) T(std::move(elem));
        ++sz;
    }

    void pop_back() {
        std::destroy_at(data + sz - 1);
        --sz;
    }

    size_t size() const noexcept {
        return sz;
    }

    size_t capacity() const noexcept {
        return data.cp;
    }

    T& operator[](size_t i) {
        return data[i];
    }

    const T& operator[](size_t i) const {
        return data[i];
    }

    auto begin() {
        return data.buffer;
    }

    auto end() {
        return data + sz;
    }

    auto begin() const {
        return data + 0;
    }

    auto end() const {
        return data + sz;
    }

    void clear() {
        resize(0);
    }
};