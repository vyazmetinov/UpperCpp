#include <iostream>
#include "TypeList.h"
#include <type_traits>

int main() {
    using L = TypeList<int, char>;
    using R = PushBackT<L, double>;

    static_assert(std::is_same_v<R, TypeList<int, char, double>>);
    static_assert(R::size == 3);

    using MyList = TypeList<int, double, char, bool>;

    static_assert(std::is_same_v<typename GetInd<MyList, 0>::type, int>);
    static_assert(std::is_same_v<typename GetInd<MyList, 1>::type, double>);
    static_assert(std::is_same_v<typename GetInd<MyList, 2>::type, char>);
    static_assert(std::is_same_v<typename GetInd<MyList, 3>::type, bool>);

    using MyList1 = TypeList<int, double, char>;

    static_assert(Find<MyList1, int>::value);
    static_assert(Find<MyList1, double>::value);
    static_assert(Find<MyList1, char>::value);
    static_assert(!Find<MyList1, bool>::value);

    using MyList2 = TypeList<int, double, char, bool>;

    static_assert(GetName<MyList2, int>::value == 0);
    static_assert(GetName<MyList2, double>::value == 1);
    static_assert(GetName<MyList2, char>::value == 2);
    static_assert(GetName<MyList2, bool>::value == 3);
    static_assert(GetName<MyList2, float>::value == -1);

    return 0;
}
