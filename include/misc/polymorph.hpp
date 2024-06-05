
#pragma once
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

        template<typename Implementation> 
        bool is() const
            requires(std::is_base_of_v<Interface, Implementation>) { 
                return cpp_type_info_str == typeid(Implementation).name(); 
        }
        
        template<typename Implementation> 
        const Implementation& get() const
            requires(std::is_base_of_v<Interface, Implementation>) { 
                
                //this function it's in "errors/internal_errors.hpp" but
                //include is impossible because it causes cyclic dependencies
                //between header files and this leads to compilation failure
                void assert_get_operation_is_possible(
                    const std::string& wanted_type, 
                    const std::type_info& type_info
                );
                
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
        std::string cpp_type_info_str;
};