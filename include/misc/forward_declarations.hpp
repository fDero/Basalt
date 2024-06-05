
#include <string>
#include <vector>
#include <typeinfo>

struct ProgramRepresentation;
struct GenericSubstitutionRuleSet;
struct TypeSignature;

void assert_get_operation_is_possible(
    const char* wanted_type, 
    const std::type_info& type_info
);