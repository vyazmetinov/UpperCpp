#pragma once

#include <optional>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <utility>

#include "../t2/TypeList.h"

template<typename... Types>
class TypeMap {
public:
    using KeyList = TypeList<Types...>;

    TypeMap() = default;

    template<typename Key, typename ValueType>
    void AddValue(ValueType&& value) {
        static_assert(Find<KeyList, Key>::value, "Key is not present in TypeMap");
        std::get<IndexOf<Key>()>(storage_) = Key(std::forward<ValueType>(value));
    }

    template<typename Key>
    Key& GetValue() {
        static_assert(Find<KeyList, Key>::value, "Key is not present in TypeMap");
        auto& slot = std::get<IndexOf<Key>()>(storage_);
        if (!slot.has_value()) {
            throw std::runtime_error("Requested key has no stored value");
        }
        return slot.value();
    }

    template<typename Key>
    const Key& GetValue() const {
        static_assert(Find<KeyList, Key>::value, "Key is not present in TypeMap");
        const auto& slot = std::get<IndexOf<Key>()>(storage_);
        if (!slot.has_value()) {
            throw std::runtime_error("Requested key has no stored value");
        }
        return slot.value();
    }

    template<typename Key>
    constexpr bool Contains() const {
        static_assert(Find<KeyList, Key>::value, "Key is not present in TypeMap");
        return std::get<IndexOf<Key>()>(storage_).has_value();
    }

    template<typename Key>
    void RemoveValue() {
        static_assert(Find<KeyList, Key>::value, "Key is not present in TypeMap");
        std::get<IndexOf<Key>()>(storage_).reset();
    }

private:
    template<typename Key>
    static constexpr std::size_t IndexOf() {
        constexpr int index = GetName<KeyList, Key>::value;
        static_assert(index >= 0, "Key is not present in TypeMap");
        return static_cast<std::size_t>(index);
    }

    std::tuple<std::optional<Types>...> storage_;
};
