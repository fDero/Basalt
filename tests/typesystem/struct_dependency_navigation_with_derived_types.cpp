
#include <gtest/gtest.h>
#include "language/generics.hpp"
#include "errors/internal_errors.hpp"
#include "toolchain/preprocessor.hpp"
#include "../tests_utilities/union_definition_factory.hpp"
#include "../tests_utilities/struct_definition_factory.hpp"
#include "../tests_utilities/typesignature_factory.hpp"

TEST(Preprocessor, Recursive_Struct_Dependency_With_Pointers_Is_Not_Cyclic_Dependency) {
    StructDefinition A = StructDefinitionFactory::make_struct_definition(
        "A", { }, {
            StructDefinition::Field { "b", TypeSignatureFactory::make_base_type("B", {}) }
        }
    );
    StructDefinition B = StructDefinitionFactory::make_struct_definition(
        "B", { }, {
            StructDefinition::Field { "a", 
                TypeSignatureFactory::make_ptr_type(
                    TypeSignatureFactory::make_base_type("A", {})    
                ) 
            }
        }
    );
    TypeDefinitionsRegister type_register;
    type_register.store(A);
    type_register.store(B);
    TypeDependencyNavigator navigator(type_register);
    navigator.visit_struct_definition(A);
}

TEST(Preprocessor, Recursive_Struct_Dependency_With_Slices_Is_Not_Cyclic_Dependency) {
    StructDefinition A = StructDefinitionFactory::make_struct_definition(
        "A", { }, {
            StructDefinition::Field { "b", TypeSignatureFactory::make_base_type("B", {}) }
        }
    );
    StructDefinition B = StructDefinitionFactory::make_struct_definition(
        "B", { }, {
            StructDefinition::Field { "a", 
                TypeSignatureFactory::make_slice_type(
                    TypeSignatureFactory::make_base_type("A", {})    
                ) 
            }
        }
    );
    TypeDefinitionsRegister type_register;
    type_register.store(A);
    type_register.store(B);
    TypeDependencyNavigator navigator(type_register);
    navigator.visit_struct_definition(A);
}

TEST(Preprocessor, Recursive_Struct_With_Arrays_Is_Cyclic_Dependency) {
    StructDefinition A = StructDefinitionFactory::make_struct_definition(
        "A", { }, {
            StructDefinition::Field { "b", TypeSignatureFactory::make_base_type("B", {}) }
        }
    );
    StructDefinition B = StructDefinitionFactory::make_struct_definition(
        "B", { }, {
            StructDefinition::Field { "a", 
                TypeSignatureFactory::make_array_type(
                    TypeSignatureFactory::make_base_type("A", {}),    
                    10
                ) 
            }
        }
    );
    TypeDefinitionsRegister type_register;
    type_register.store(A);
    type_register.store(B);
    TypeDependencyNavigator navigator(type_register);
    ASSERT_ANY_THROW({
        navigator.visit_struct_definition(A);
    });
}