
#include <gtest/gtest.h>
#include "toolchain/rappresentation.hpp"
#include "language/syntax.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "../tests_utilities/rappresentation_inline_procs.hpp"

TEST(Rappresentation, Trivial_Function_Store_And_Retrieval_No_Args_No_Generics) {
    FunctionDefinitionsRegister func_register;
    FunctionDefinition func_definition = make_function_definition("myfunc");
    func_register.store(func_definition);  
    FunctionDefinition retrieved_func = func_register.retrieve("myfunc", {});
    EXPECT_EQ(retrieved_func.function_name, func_definition.function_name);
    EXPECT_EQ(retrieved_func.arguments.size(), func_definition.arguments.size());
    EXPECT_EQ(retrieved_func.template_generics_names, func_definition.template_generics_names);
}

TEST(Rappresentation, Trivial_Function_Store_And_Retrieval_One_Arg_No_Generics) {
    FunctionDefinitionsRegister func_register;
    FunctionDefinition func_definition = make_function_definition("myfunc");
    CustomType Int { "Int", {} };
    func_definition.arguments.push_back(FunctionDefinition::Argument{ "x", Int });
    func_register.store(func_definition);  
    FunctionDefinition retrieved_func = func_register.retrieve("myfunc", { Int });
    EXPECT_EQ(retrieved_func.function_name, func_definition.function_name);
    EXPECT_EQ(retrieved_func.arguments.size(), func_definition.arguments.size());
    EXPECT_EQ(retrieved_func.template_generics_names, func_definition.template_generics_names);
}

TEST(Rappresentation, Trivial_Function_Store_And_Retrieval_Two_Args_No_Generics) {
    FunctionDefinitionsRegister func_register;
    FunctionDefinition func_definition = make_function_definition("myfunc");
    CustomType Int { "Int", {} };
    CustomType Float { "Float", {} };
    func_definition.arguments.push_back(FunctionDefinition::Argument{ "x", Int });
    func_definition.arguments.push_back(FunctionDefinition::Argument{ "y", Float });
    func_register.store(func_definition);  
    FunctionDefinition retrieved_func = func_register.retrieve("myfunc", { Int, Float });
    EXPECT_EQ(retrieved_func.function_name, func_definition.function_name);
    EXPECT_EQ(retrieved_func.arguments.size(), func_definition.arguments.size());
    EXPECT_EQ(retrieved_func.template_generics_names, func_definition.template_generics_names);
}

TEST(Rappresentation, Trivial_Function_Store_And_Retrieval_Complex_Args_No_Generics) {
    FunctionDefinitionsRegister func_register;
    FunctionDefinition func_definition = make_function_definition("myfunc");
    CustomType Node { CustomType { "ListNode", { CustomType { "Int", {} } } } };
    PointerType StringPtr { CustomType { "String", {} } };
    func_definition.arguments.push_back(FunctionDefinition::Argument{ "node", Node });
    func_definition.arguments.push_back(FunctionDefinition::Argument{ "strptr", StringPtr });
    func_register.store(func_definition);  
    FunctionDefinition retrieved_func = func_register.retrieve("myfunc", { Node, StringPtr });
    EXPECT_EQ(retrieved_func.function_name, func_definition.function_name);
    EXPECT_EQ(retrieved_func.arguments.size(), func_definition.arguments.size());
    EXPECT_EQ(retrieved_func.template_generics_names, func_definition.template_generics_names);
}