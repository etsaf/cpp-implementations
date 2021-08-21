//simplified std::optional implementation

#include <iostream>

struct BadOptionalAccess {
};

template <typename T>
class Optional {
private:
    alignas(T) unsigned char data[sizeof(T)];
    bool defined = false;

public:
    Optional() = default;

    Optional(const T& elem) {
        new (data) T(elem);
        defined = true;
    }

    Optional(T && elem) {
        new (data) T(std::move(elem));
        defined = true;
    }

    Optional(const Optional& other) {
        if (other.defined) {
            new (data) T(other.value());
            defined = true;
        }
    }

    Optional& operator=(const Optional& other) {
        if (defined) {
            if (other.defined) {
                value() = other.value();
            } else {
                reset();
            }
        } else {
            if (other.defined) {
                new (data) T(other.value());
                defined = true;
            }
        }
        return *this;
    }

    Optional& operator=(const T& elem) {
        if (defined) {
            value() = elem;
        } else {
            new (data) T(elem);
            defined = true;
        }
        return *this;
    }

    Optional& operator=(T&& elem) {
        if (defined) {
            value() = std::move(elem);
        } else {
            new (data) T(std::move(elem));
            defined = true;
        }
        return *this;
    }

    bool has_value() const {
        return defined;
    }

    T& operator*() {
        return value();
    }

    const T& operator*() const {
        return value();
    }

    T* operator->() {
        if (!defined) {
            throw BadOptionalAccess();
        }
        return reinterpret_cast<T*>(data);
    }

    const T* operator->() const {
        if (!defined) {
            throw BadOptionalAccess();
        }
        return reinterpret_cast<const T*>(data);
    }

    T& value() {
        if (!defined) {
            throw BadOptionalAccess();
        }
        return *reinterpret_cast<T*>(data);
    }

    const T& value() const {
        if (!defined) {
            throw BadOptionalAccess();
        }
        return *reinterpret_cast<const T*>(data);
    }

    void reset() {
        if (defined) {
            value().~T();
            defined = false;
        }
    }

    ~Optional() {
        reset();
    }
};