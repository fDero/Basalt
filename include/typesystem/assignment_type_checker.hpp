//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include "model/type_definitions_register.hpp"
#include "model/project_file_structure.hpp"
#include "typesystem/generics_substitution_rules.hpp"

class AssignmentTypeChecker {

    public:
        AssignmentTypeChecker(TypeDefinitionsRegister& program_representation, ProjectFileStructure& project_file_structure);
        bool validate_assignment(const TypeSignature& source, const TypeSignature& dest);
        GenericSubstitutionRule::Set::Ref get_generic_substitution_rules(); 

    private:
        bool validate_type_alias_unaware_assignment(const TypeSignature& source, const TypeSignature& dest);
        bool validate_assignment_to_slice_type(const TypeSignature& source, const SliceType& dest);
        bool validate_assignment_to_pointer_type(const TypeSignature& source, const PointerType& dest);
        bool validate_assignment_to_array_type(const TypeSignature& source, const ArrayType& dest);
        bool validate_assignment_to_primitive_type(const TypeSignature& source, const PrimitiveType& dest);
        bool validate_assignment_to_custom_type(const TypeSignature& source, const CustomType& dest);
        bool validate_assignment_to_template_generic(const TypeSignature& source, const TemplateType& dest);
        bool name_equivalence_assignment_validation(const CustomType& source, const CustomType& dest);
        bool structural_equivalence_assignment_validation(const TypeSignature& source, const TypeSignature& dest);
        bool validate_assignment_to_string(const TypeSignature& source, const PrimitiveType& dest);
        bool validate_assignment_to_inline_union(const TypeSignature& source, const InlineUnion& inline_union);
        bool validate_assignment_to_generic_type_parameter(const TypeSignature& source, const TemplateType& dest);
        bool validate_assignment_very_strictly(const TypeSignature& source_generic, const TypeSignature& dest_generic);
        bool validate_assignment_to_string_from_primitive_type(const PrimitiveType& source, const PrimitiveType& dest);
        bool validate_assignment_to_string_from_slice_type(const SliceType& source, const PrimitiveType& dest);
        bool validate_assignment_to_string_from_array_type(const ArrayType& source, const PrimitiveType& dest);

        std::vector<TypeSignature> try_to_get_union_alternatives(const TypeSignature& maybe_union_type_signature);
        bool validate_assignment_to_union_alternatives(const TypeSignature& source, const std::vector<TypeSignature>& alternatives);

        TypeDefinitionsRegister& type_definitions_register;
        ProjectFileStructure& project_file_structure;
        GenericSubstitutionRule::Set::Ref generic_substitution_rules = std::make_shared<GenericSubstitutionRule::Set>();

        bool type_parameters_assignment_validation(const CustomType &source, const CustomType &dest);
};