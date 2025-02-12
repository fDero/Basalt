//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <set>

#include "core/type_definitions_register.hpp"
#include "core/project_file_structure.hpp"
#include "core/generics_substitution_rules.hpp"

class AssignmentTypeChecker {

    public:
        AssignmentTypeChecker(
            TypeDefinitionsRegister& program_representation, 
            ProjectFileStructure& project_file_structure
        );

        [[nodiscard]] bool validate_assignment(
            const TypeSignature& source, 
            const TypeSignature& dest,
            bool strict_mode = false
        );
        
        [[nodiscard]] GenericSubstitutionRule::Set::Ref 
            get_generic_substitution_rules(); 

    private:
        [[nodiscard]] bool name_equivalence_assignment_validation(
            const CustomType& source, 
            const CustomType& dest,
            bool strict_mode
        );
        
        [[nodiscard]] bool structural_equivalence_assignment_validation(
            const TypeSignature& source, 
            const TypeSignature& dest,
            bool strict_mode
        );
        
        [[nodiscard]] bool type_parameters_assignment_validation(
            const CustomType &source, 
            const CustomType &dest,
            bool strict_mode
        );
        
        [[nodiscard]] bool validate_type_alias_unaware_assignment(
            const TypeSignature& source, 
            const TypeSignature& dest,
            bool strict_mode
        );
        
        [[nodiscard]] bool validate_assignment_to_slice_type(
            const TypeSignature& source, 
            const SliceType& dest,
            bool strict_mode
        );
        
        [[nodiscard]] bool validate_assignment_to_pointer_type(
            const TypeSignature& source, 
            const PointerType& dest,
            bool strict_mode
        );
        
        [[nodiscard]] bool validate_assignment_to_array_type(
            const TypeSignature& source, 
            const ArrayType& dest, 
            bool strict_mode
        );
        
        [[nodiscard]] bool validate_assignment_to_primitive_type(
            const TypeSignature& source, 
            const PrimitiveType& dest,
            bool strict_mode
        );
        
        [[nodiscard]] bool validate_assignment_to_custom_type(
            const TypeSignature& source, 
            const CustomType& dest,
            bool strict_mode
        );
        
        [[nodiscard]] bool validate_assignment_to_string(
            const TypeSignature& source, 
            const PrimitiveType& dest,
            bool strict_mode
        );
        
        [[nodiscard]] bool validate_assignment_to_inline_union(
            const TypeSignature& source, 
            const InlineUnion& inline_union,
            bool strict_mode
        );
        
        [[nodiscard]] bool validate_assignment_to_generic_type_parameter(
            const TypeSignature& source, 
            const TemplateType& dest,
            bool strict_mode
        );
        
        [[nodiscard]] bool validate_assignment_to_string_from_primitive_type(
            const PrimitiveType& source, 
            const PrimitiveType& dest,
            bool strict_mode
        );
        
        [[nodiscard]] bool validate_assignment_to_string_from_slice_type(
            const SliceType& source, 
            const PrimitiveType& dest, 
            bool strict_mode
        );
        
        [[nodiscard]] bool validate_assignment_to_union_alternatives(
            const TypeSignature& source, 
            const std::vector<TypeSignature>& alternatives,
            bool strict_mode
        );
        
        [[nodiscard]] bool validate_assignment_to_string_from_pointer_type(
            const PointerType& pointer_type, 
            const PrimitiveType& dest,
            bool strict_mode
        );

        [[nodiscard]] bool validate_assignment_to_slice_from_pointer_type(
            const PointerType& pointer_type, 
            const SliceType& dest,
            bool strict_mode
        );

        [[nodiscard]] bool validate_assignment_to_template_generic(
            const TypeSignature& source, 
            const TemplateType& dest,
            bool strict_mode
        );
        
        TypeDefinitionsRegister& type_definitions_register;
        ProjectFileStructure& project_file_structure;
        std::set<std::string> strict_type_inference_deductions;       
       
        GenericSubstitutionRule::Set::Ref generic_substitution_rules 
            = std::make_shared<GenericSubstitutionRule::Set>();
};