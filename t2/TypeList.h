#pragma once

#include <cstddef>

template<typename... Types>
class TypeList {
public:
    constexpr TypeList() = default;

    static constexpr std::size_t size = sizeof...(Types);
};

namespace detail {
    template<typename List, typename NewType>
    struct PushBackImpl;

    template<typename... Types, typename NewType>
    struct PushBackImpl<TypeList<Types...>, NewType> {
        using type = TypeList<Types..., NewType>;
    };

    template<typename List, typename NewType>
    using PushBackImplT = typename PushBackImpl<List, NewType>::type;


    template<typename List, typename NewType>
    struct PushFrontImpl;

    template<typename... Types, typename NewType>
    struct PushFrontImpl<TypeList<Types...>, NewType> {
        using type = TypeList<NewType, Types...>;
    };

    template<typename List, typename NewType>
    using PushFrontImplT = typename PushFrontImpl<List, NewType>::type;

    template<typename List, std::size_t ind>
    struct GetIndImpl;

    template<typename Head, typename... Tail>
    struct GetIndImpl<TypeList<Head, Tail...>, 0> {
        using type = Head;
    };

    template<typename Head, typename... Tail, std::size_t ind>
    struct GetIndImpl<TypeList<Head, Tail...>, ind> {
        static_assert(ind < sizeof...(Tail) + 1, "index out of range");
        using type = typename GetIndImpl<TypeList<Tail...>, ind - 1>::type;
    };

    template<typename List, typename name>
    struct FindImpl;

    template<typename name>
    struct FindImpl<TypeList<>, name> {
        static constexpr auto value = false;
    };

    template<typename... Tail, typename name>
    struct FindImpl<TypeList<name, Tail...>, name> {
        static constexpr auto value = true;
    };

    template<typename Head, typename... Tail, typename name>
    struct FindImpl<TypeList<Head, Tail...>, name> {
        static constexpr auto value = FindImpl<TypeList<Tail...>, name>::value;
    };


    template<typename List, typename name>
    struct GetNameImpl;

    template<typename name>
    struct GetNameImpl<TypeList<>, name> {
        static constexpr auto value = -1;
    };

    template<typename... Tail, typename name>
    struct GetNameImpl<TypeList<name, Tail...>, name> {
        static constexpr auto value = 0;
    };

    template<typename Head, typename... Tail, typename name>
    struct GetNameImpl<TypeList<Head, Tail...>, name> {
    private:
        static constexpr auto next = GetNameImpl<TypeList<Tail...>, name>::value;

    public:
        static constexpr auto value = (next == -1 ? -1 : next + 1);
    };
}

template<typename List, typename NewType>
struct PushBack {
    using type = typename detail::PushBackImpl<List, NewType>::type;
};

template<typename List, typename NewType>
using PushBackT = typename PushBack<List, NewType>::type;

template<typename List, typename NewType>
struct PushFront {
    using type = typename detail::PushFrontImpl<List, NewType>::type;
};

template<typename List, typename NewType>
using PushFrontT = typename PushFront<List, NewType>::type;

template<typename List, std::size_t ind>
struct GetInd {
    using type = typename detail::GetIndImpl<List, ind>::type;
};

template<typename List, typename Name>
struct Find {
    static constexpr bool value = detail::FindImpl<List, Name>::value;
};

template<typename List, typename Name>
struct GetName {
    static constexpr int value = detail::GetNameImpl<List, Name>::value;
};
