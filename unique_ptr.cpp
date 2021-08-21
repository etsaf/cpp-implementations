//simplified std::unique_ptr implementation

#include <memory>
#include <iostream>
#include <utility>
#include <cstddef>

template <typename T, class Deleter = std::default_delete<T>>
class UniquePtr {
private:
    std::tuple<T*, Deleter> combined;

public:
    UniquePtr() {
        std::get<0>(combined) = nullptr;
    }

    UniquePtr(T* t) {
        std::get<0>(combined) = t;
    }

    UniquePtr(T* t, Deleter d) {
        std::get<0>(combined) = t;
        std::get<1>(combined) = d;
    }

    UniquePtr(const UniquePtr& other) = delete;

    UniquePtr(UniquePtr&& other) {
        swap(other);
    }

    UniquePtr& operator=(std::nullptr_t) {
        std::get<1>(combined)(std::get<0>(combined));
        std::get<0>(combined) = nullptr;
        return *this;
    }

    UniquePtr& operator=(const UniquePtr& other) = delete;

    UniquePtr& operator=(UniquePtr&& other) {
        reset(other.release());
        std::get<1>(combined) = std::move(std::get<1>(other.combined));
        return *this;
    }

    ~UniquePtr() {
        std::get<1>(combined)(std::get<0>(combined));
    }

    const T& operator*() const {
        return *std::get<0>(combined);
    }

    const T* operator->() const {
        return std::get<0>(combined);
    }

    T* release() noexcept {
        T* tmp = std::get<0>(combined);
        std::get<0>(combined) = nullptr;
        return tmp;
    }

    void reset(T* ptr) {
        std::get<1>(combined)(std::get<0>(combined));
        std::get<0>(combined) = ptr;
    }

    void swap(UniquePtr& other) noexcept {
        std::swap(std::get<0>(combined), std::get<0>(other.combined));
        std::swap(std::get<1>(combined), std::get<1>(other.combined));
    }

    T* get() const {
        return std::get<0>(combined);
    }

    explicit operator bool() const {
        return (std::get<0>(combined) != nullptr);
    }

    const Deleter& get_deleter() const {
        return std::get<1>(combined);
    }

    Deleter& get_deleter() {
        return std::get<1>(combined);
    }
};