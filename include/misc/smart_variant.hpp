/**
 * @file smart_variant.hpp
 * @author Francesco De Rosa (francescodero@outlook.it)
 * @brief This file contains the definition of the SmartVariant struct
 * @version 0.1
 * @date 2024-09-01
 * 
 * @copyright Copyright (c) 2024
 */

#pragma once

#include <variant>

/**
 * @brief   A variant that is aware of the types it contains.
 * 
 * @details The SmartVariant struct is a variant that is aware of the types it contains.
 *          It is a wrapper around std::variant that provides some utility methods to
 *          check the type of the contained value and to get it. It's designed to 
 *          expose the very same API as Polymorph
 * 
 * @see     Polymorph
 * 
 * @tparam Types The types that the variant can contain.
 */
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