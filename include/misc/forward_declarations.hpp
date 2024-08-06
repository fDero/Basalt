
#include <string>
#include <vector>
#include <typeinfo>

struct TypeDefinitionsRegister;
struct GenericSubstitutionRuleSet;
struct TypeSignature;
struct Expression;

void assert_get_operation_is_possible(
    const char* wanted_type, 
    const std::type_info& type_info
);