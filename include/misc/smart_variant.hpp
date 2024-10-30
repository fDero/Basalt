//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <variant>

template <typename...Types>
struct SmartVariant : public std::variant<Types...> {

    using std::variant<Types...>::variant;
    using std::variant<Types...>::operator=;
    using std::variant<Types...>::index;
    
    template <typename T> [[nodiscard]] bool is() const {
        return std::holds_alternative<T>(*this);
    }

    template <typename T> [[nodiscard]] const T& get() const {
        return std::get<T>(*this);
    }

    template <typename T> [[nodiscard]] T& get() {
        return std::get<T>(*this);
    }
};