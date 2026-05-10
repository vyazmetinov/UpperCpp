#include "TypeMap.h"

#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>

struct DataA {
    std::string value;
};

struct DataB {
    int value;
};

int main() {
    TypeMap<int, double, DataA, DataB> myTypeMap;

    static_assert(std::is_same_v<decltype(myTypeMap.GetValue<int>()), int&>);
    static_assert(std::is_same_v<decltype(myTypeMap.GetValue<DataA>()), DataA&>);

    myTypeMap.AddValue<int>(42);
    myTypeMap.AddValue<double>(3.14);
    myTypeMap.AddValue<DataA>(DataA{"Hello, TypeMap!"});
    myTypeMap.AddValue<DataB>(DataB{10});

    assert(myTypeMap.Contains<int>());
    assert(myTypeMap.Contains<double>());
    assert(myTypeMap.Contains<DataA>());
    assert(myTypeMap.Contains<DataB>());

    assert(myTypeMap.GetValue<int>() == 42);
    assert(myTypeMap.GetValue<double>() == 3.14);
    assert(myTypeMap.GetValue<DataA>().value == "Hello, TypeMap!");
    assert(myTypeMap.GetValue<DataB>().value == 10);

    myTypeMap.RemoveValue<double>();
    assert(!myTypeMap.Contains<double>());

    bool thrown = false;
    try {
        (void)myTypeMap.GetValue<double>();
    } catch (const std::runtime_error&) {
        thrown = true;
    }
    assert(thrown);

    std::cout << "All tests passed\n";
    return 0;
}
