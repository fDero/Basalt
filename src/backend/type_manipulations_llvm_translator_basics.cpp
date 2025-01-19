//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "backend/type_manipulations_llvm_translator.hpp"
#include "backend/llvm_wrappers.hpp"
#include "errors/internal_errors.hpp"

TypeManipulationsLLVMTranslator::TypeManipulationsLLVMTranslator(
    ProgramRepresentation& program_representation, 
    TypeDefinitionsLLVMTranslator& type_definitions_llvm_translator
)
    : program_representation(program_representation)
    , type_definitions_llvm_translator(type_definitions_llvm_translator)
{ }


TypeManipulationsLLVMTranslator::CastStrategy 
TypeManipulationsLLVMTranslator::compute_cast_strategy(
    const TypeSignature& source,
    const TypeSignature& dest
) {
    llvm::Type* source_llvm_type = type_definitions_llvm_translator.translate_typesignature_to_llvm_type(source);
    llvm::Type* dest_llvm_type = type_definitions_llvm_translator.translate_typesignature_to_llvm_type(dest);
    if (source_llvm_type == dest_llvm_type) return CastStrategy::noop;

    bool source_is_union = program_representation.is_union(source);
    bool dest_is_union = program_representation.is_union(dest);
    if (source_is_union && dest_is_union)  return CastStrategy::union_to_union;
    if (source_is_union && !dest_is_union) return CastStrategy::union_to_alternative;
    if (!source_is_union && dest_is_union) return CastStrategy::alternative_to_union;
    
    bool source_is_ptr = source.is<PointerType>();
    bool source_is_ptr_to_array = source_is_ptr && source.get<PointerType>().pointed_type.is<ArrayType>();
    bool dest_is_slice = dest.is<SliceType>();
    bool dest_is_string = dest.is<PrimitiveType>() && dest.get<PrimitiveType>().type_name == "String";
    bool dest_is_raw_string = dest.is<PrimitiveType>() && dest.get<PrimitiveType>().type_name == "RawString";
    if (source_is_ptr_to_array && dest_is_slice) return CastStrategy::array_pointer_to_slice;
    if (source_is_ptr_to_array && dest_is_string) return CastStrategy::array_pointer_to_string;
    if (source_is_ptr_to_array && dest_is_raw_string) return CastStrategy::array_pointer_to_raw_string;
   
    bool source_is_slice = source.is<SliceType>();
    if (source_is_slice && dest_is_string) return CastStrategy::slice_to_string;
    if (source_is_slice && dest_is_raw_string) return CastStrategy::slice_to_raw_string;;

    bool source_is_string = source.is<PrimitiveType>() && source.get<PrimitiveType>().type_name == "String";
    if (source_is_string && dest_is_raw_string) return CastStrategy::string_to_raw_string;

    bool source_is_array = source.is<ArrayType>();
    bool dest_is_array = dest.is<ArrayType>();
    if (source_is_array && dest_is_array) return CastStrategy::array_to_array;

    return CastStrategy::noop;
}

TranslatedExpression TypeManipulationsLLVMTranslator::cast_translated_expression_to_another_type_in_llvm(
    llvm::BasicBlock* block,
    TranslatedExpression expression,
    const TypeSignature& soruce,
    const TypeSignature& dest
) {
    switch (compute_cast_strategy(soruce, dest)) {
        case CastStrategy::noop: return expression;
        case CastStrategy::union_to_union: return cast_union_expression_to_another_union_type_in_llvm(block, expression, dest);
        case CastStrategy::union_to_alternative: return cast_union_expression_to_one_of_its_alternatives_in_llvm(block, expression, dest);
        case CastStrategy::alternative_to_union: return cast_simple_expression_to_union_in_llvm(block, expression, soruce, dest);
        case CastStrategy::array_pointer_to_slice: return cast_pointer_to_array_to_slice_in_llvm(block, expression, dest, soruce.get<PointerType>());
        case CastStrategy::array_pointer_to_string: {
            TranslatedExpression step1 = cast_pointer_to_array_to_slice_in_llvm(block, expression, dest, soruce.get<PointerType>());
            return cast_slice_of_chars_to_string_llvm(block, step1);
        }
        case CastStrategy::array_pointer_to_raw_string: {
            TranslatedExpression step1 = cast_pointer_to_array_to_slice_in_llvm(block, expression, dest, soruce.get<PointerType>());
            TranslatedExpression step2 = cast_slice_of_chars_to_string_llvm(block, step1);
            return cast_string_to_raw_string_in_llvm(block, step2);
        }
        case CastStrategy::slice_to_string: {
            return cast_slice_of_chars_to_string_llvm(block, expression);
        }
        case CastStrategy::slice_to_raw_string: {
            TranslatedExpression step1 = cast_slice_of_chars_to_string_llvm(block, expression);
            return cast_string_to_raw_string_in_llvm(block, step1);
        }
        case CastStrategy::string_to_raw_string: {
            return cast_string_to_raw_string_in_llvm(block, expression);
        }
        case CastStrategy::array_to_array: {
            const ArrayType& src_array_type = soruce.get<ArrayType>();
            const ArrayType& dest_array_type = dest.get<ArrayType>();
            return cast_array_to_array_of_another_type_in_llvm(block, expression, src_array_type, dest_array_type);
        }
    }
    return expression;
}