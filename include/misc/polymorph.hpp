/**
 * @file polymorph.hpp
 * @author Francesco De Rosa (francescodero@outlook.it)
 * @brief This file contains the definition of the Polymorph class
 * @version 0.1
 * @date 2024-09-01
 * 
 * @copyright Copyright (c) 2024
 */

#pragma once

#include "forward_declarations.hpp"

#include <memory>
#include <string>

/**
 * @brief   This class is a wrapper on top of std::shared_ptr that allows to store polymorphic types with 
 *          value semantics while still being able to access the underlying type. Moreover, it allows to
 *          check the type of the stored object at runtime in O(1) time and to retrieve the stored object.
 *
 * @tparam  Interface The base class of the polymorphic types that will be stored in the Polymorph object. 
 
 *  
 * @details The Polymorph class is a wrapper on top of std::shared_ptr that allows to store polymorphic types with 
 *          value semantics while still being able to access the underlying type. Moreover, it allows to
 *          check the type of the stored object at runtime in O(1) time and to retrieve the stored object.
 *          It's used trough all the project to store polymorphic types such as definitions, statements, expressions etc...
 * 
 * @note    The Polymorph class works even with types that do not have a virtual destructor, since it uses
 *          std::shared_ptr to manage the memory of the stored object, wich internally have its own way to
 *          destruct an object that does not involve calling its destructor directly. It's not recommended to
 *          exploit this behaviour, since the internal mechanism of this class is not guaranteed to stay the same
 *          forever, and it's better to always define a virtual destructor in the base class of the polymorphic types.
 * 
 */
template<typename Interface>
class Polymorph {
    public:

        Polymorph(const Polymorph& other)
            : ptr(other.ptr), cpp_type_info_str(other.cpp_type_info_str) { }

        template<typename Implementation>
        Polymorph(const Implementation& type)
            requires(std::is_base_of_v<Interface, Implementation>) {
                ptr = std::make_shared<Implementation>(type);
                cpp_type_info_str = typeid(Implementation).name();
        }

        Polymorph& operator=(const Polymorph& other) {
            ptr = other.ptr;
            cpp_type_info_str = other.cpp_type_info_str;
            return *this;
        }

        template<typename Implementation>
        bool is() const
            requires(std::is_base_of_v<Interface, Implementation>) {
                return cpp_type_info_str == typeid(Implementation).name();
        }

        template<typename Implementation>
        const Implementation& get() const
            requires(std::is_base_of_v<Interface, Implementation>) {
                assert_get_operation_is_possible(cpp_type_info_str, typeid(Implementation));
                return *static_cast<Implementation*>(ptr.get());
        }

        template<typename Implementation>
        Implementation& get()
            requires(std::is_base_of_v<Interface, Implementation>) {
                return const_cast<Implementation&>(
                    static_cast<const Polymorph&>(*this).get<Implementation>()
                );
        }

    protected:
        std::shared_ptr<Interface> ptr;
        const char* cpp_type_info_str;
};
