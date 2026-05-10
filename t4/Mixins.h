#pragma once

#include <cstddef>

template<typename Derived>
class less_than_comparable {
public:
    friend bool operator>(const Derived& lhs, const Derived& rhs) { return rhs < lhs; }
    friend bool operator<=(const Derived& lhs, const Derived& rhs) { return !(rhs < lhs); }
    friend bool operator>=(const Derived& lhs, const Derived& rhs) { return !(lhs < rhs); }
    friend bool operator==(const Derived& lhs, const Derived& rhs) { return !(lhs < rhs) && !(rhs < lhs); }
    friend bool operator!=(const Derived& lhs, const Derived& rhs) { return !(lhs == rhs); }
};

template<typename Derived>
class counter {
public:
    counter() { ++created_count_; }
    counter(const counter&) { ++created_count_; }
    counter(counter&&) noexcept { ++created_count_; }
    counter& operator=(const counter&) = default;
    counter& operator=(counter&&) noexcept = default;
    ~counter() = default;

    static std::size_t count() { return created_count_; }

private:
    inline static std::size_t created_count_ = 0;
};
