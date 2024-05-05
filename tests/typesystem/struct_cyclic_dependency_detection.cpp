
#include <gtest/gtest.h>
#include "language/generics.hpp"
#include "errors/internal_errors.hpp"
#include "toolchain/preprocessor.hpp"
#include "../tests_utilities/union_definition_factory.hpp"
#include "../tests_utilities/struct_definition_factory.hpp"
#include "../tests_utilities/typesignature_factory.hpp"

TEST(Preprocessor, Cyclic_Dependency_Type_Navigation_On_Non_Generic_Structs) {
    StructDefinition A = StructDefinitionFactory::make_struct_definition(
        "A", { }, {
            StructDefinition::Field { "b", TypeSignatureFactory::make_base_type("B", {}) }
        }
    );
    StructDefinition B = StructDefinitionFactory::make_struct_definition(
        "B", { }, {
            StructDefinition::Field { "a", TypeSignatureFactory::make_base_type("A", {}) }
        }
    );
    TypeDefinitionsRegister type_register;
    type_register.store(A);
    type_register.store(B);
    TypeDependencyNavigator navigator(type_register);
    EXPECT_ANY_THROW({
        navigator.visit_struct_definition(A);
    });
}

TEST(Preprocessor, Cyclic_Dependency_Type_Navigation_On_Generic_Structs) {
    StructDefinition Wrapper = StructDefinitionFactory::make_struct_definition(
        "Wrapper", 
        { "T" }, 
        {
            StructDefinition::Field { "b", TypeSignatureFactory::T }
        }
    );
    StructDefinition MyStruct = StructDefinitionFactory::make_struct_definition(
        "MyStruct", { }, {
            StructDefinition::Field { "wrapper", 
                TypeSignatureFactory::make_base_type("Wrapper", { 
                    TypeSignatureFactory::make_base_type("B", {}) 
                })
            }
        }
    );
    TypeDefinitionsRegister type_register;
    type_register.store(Wrapper);
    type_register.store(MyStruct);
    TypeDependencyNavigator navigator(type_register);
    EXPECT_ANY_THROW({
        navigator.visit_struct_definition(MyStruct);
    });
}

TEST(Preprocessor, Cyclic_Dependency_Type_Navigation_On_Three_Generic_Structs) {
    StructDefinition Pair = StructDefinitionFactory::make_struct_definition(
        "Pair", 
        { "T" , "U" }, 
        {
            StructDefinition::Field { "first", TypeSignatureFactory::T },
            StructDefinition::Field { "second", TypeSignatureFactory::U }
        }
    );
    StructDefinition B = StructDefinitionFactory::make_struct_definition(
        "B", 
        { "T" },
        {
            StructDefinition::Field { "pair", 
                TypeSignatureFactory::make_base_type("Pair", { 
                    TypeSignatureFactory::make_base_type("B", {}),
                    TypeSignatureFactory::T 
                })
            }
        }
    );
    StructDefinition A = StructDefinitionFactory::make_struct_definition(
        "A", { },
        {
            StructDefinition::Field { "b", 
                TypeSignatureFactory::make_base_type("B", { 
                    TypeSignatureFactory::String
                })
            }
        }
    );
    TypeDefinitionsRegister type_register;
    type_register.store(Pair);
    type_register.store(B);
    type_register.store(A);
    TypeDependencyNavigator navigator(type_register);
    EXPECT_ANY_THROW({
        navigator.visit_struct_definition(A);
    });
}