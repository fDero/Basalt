//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include "misc/forward_declarations.hpp"

#include <memory>
#include <string>

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
