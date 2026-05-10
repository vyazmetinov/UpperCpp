#include "Mixins.h"

#include <cassert>
#include <iostream>

class Number : public less_than_comparable<Number>, public counter<Number> {
public:
    explicit Number(int value) : value_(value) {}

    bool operator<(const Number& other) const { return value_ < other.value_; }
    int value() const { return value_; }

private:
    int value_;
};

int main() {
    Number one{1};
    Number two{2};
    Number three{3};
    Number four{4};

    assert(one >= one);
    assert(three <= four);
    assert(two == two);
    assert(three > two);
    assert(one < two);
    assert(one != two);

    std::cout << "Count: " << counter<Number>::count() << '\n';
    assert(counter<Number>::count() == 4);
    return 0;
}
