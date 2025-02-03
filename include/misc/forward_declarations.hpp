//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include <string>
#include <vector>
#include <typeinfo>

struct TypeDefinitionsRegister;
struct GenericSubstitutionRule;
struct TypeSignature;
struct Expression;

void assert_get_operation_is_possible(
    const char* wanted_type, 
    const std::type_info& type_info
);

struct CommonFeatureAdoptionPlan;
class CallableCodeBlocksLLVMTranslator;
class ProgramRepresentation;
struct Token;