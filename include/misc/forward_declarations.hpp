//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <typeinfo>


// Forward declarations of the types that headers mostly pass around by
// reference or pointer: include this instead of the full definitions
// whenever a complete type is not required.

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ FRONTEND ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

struct Token;
struct FileRepresentation;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ TYPE SIGNATURES ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

struct TypeSignature;
struct CustomType;
struct InlineUnion;
struct TemplateType;
struct PrimitiveType;
struct PointerType;
struct ArrayType;
struct SliceType;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ EXPRESSIONS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

class Expression;
struct DotMemberAccess;
struct SquareBracketsAccess;
struct ArrayLiteral;
struct TypeOperator;
struct BinaryOperator;
struct UnaryOperator;
struct Identifier;
struct StringLiteral;
struct IntLiteral;
struct FloatLiteral;
struct BoolLiteral;
struct CharLiteral;
struct FunctionCall;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ STATEMENTS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

struct Statement;
struct VariableDeclaration;
struct ConstDeclaration;
struct Assignment;
struct Conditional;
struct WhileLoop;
struct UntilLoop;
struct Return;
struct Break;
struct Continue;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DEFINITIONS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

struct FunctionDefinition;
struct StructDefinition;
struct UnionDefinition;
struct TypeAlias;
struct TypeDefinition;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CORE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

class ProjectFileStructure;
class ProgramRepresentation;
class ScopeContext;
class TypeDefinitionsRegister;
class FunctionDefinitionsRegister;
class CommonFeatureAdoptionPlanGenerationEngine;
class ExpressionTypeDeducer;
struct GenericSubstitutionRule;
struct CommonFeatureAdoptionPlan;
struct RecursiveAdoptionPlan;
struct CommonFeatureAdoptionPlanDescriptor;
struct CallableCodeBlock;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ BACKEND ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

class TypeDefinitionsLLVMTranslator;
class CallableCodeBlocksLLVMTranslator;
class ExpressionsAndStatementsLLVMTranslator;
struct TranslationAwareScopeContext;
struct TranslatedExpression;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ MISC ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

void assert_get_operation_is_possible(
    const char* wanted_type,
    const std::type_info& type_info
);
