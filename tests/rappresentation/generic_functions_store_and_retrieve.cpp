
#include <gtest/gtest.h>
#include "toolchain/rappresentation.hpp"
#include "language/syntax.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "../tests_utilities/rappresentation_inline_procs.hpp"

TEST(Rappresentation, Generic_Function_Store_And_Retrieval_One_Generic_Arg) {
    FunctionDefinitionsRegister func_register;  
    FunctionDefinition func_definition = make_function_definition("myfunc");
    CustomType generic { "T", {} };
    CustomType Int { "Int", {} };
    func_definition.template_generics_names.push_back(generic.type_name);
    func_definition.arguments.push_back(FunctionDefinition::Argument{ "x", generic });
    func_register.store(func_definition);  
    FunctionDefinition retrieved_func = func_register.retrieve("myfunc", { Int });
    EXPECT_EQ(retrieved_func.function_name, func_definition.function_name);
    EXPECT_EQ(retrieved_func.arguments.size(), func_definition.arguments.size());
    EXPECT_EQ(retrieved_func.template_generics_names, func_definition.template_generics_names);
}

TEST(Rappresentation, Generic_Function_Store_And_Retrieval_One_Complex_Generic_Arg) {
    FunctionDefinitionsRegister func_register;  
    FunctionDefinition func_definition = make_function_definition("myfunc");
    CustomType generic { "T", {} };
    CustomType Int { "Int", {} };
    CustomType ListOfInt { "List", { Int } };
    CustomType ListOfT { "List", { generic } };
    func_definition.template_generics_names.push_back(generic.type_name);
    func_definition.arguments.push_back(FunctionDefinition::Argument{ "x", ListOfT });
    func_register.store(func_definition);  
    FunctionDefinition retrieved_func = func_register.retrieve("myfunc", { ListOfInt });
    EXPECT_EQ(retrieved_func.function_name, func_definition.function_name);
    EXPECT_EQ(retrieved_func.arguments.size(), func_definition.arguments.size());
    EXPECT_EQ(retrieved_func.template_generics_names, func_definition.template_generics_names);
}