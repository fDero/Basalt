
#include <string>
#include <vector>

struct GenericSubstitutionRuleSet;
struct TypeSignature;

using FileName = std::string;
using PackageName = std::string;

using TemplateGenerics = std::vector<std::string>;
using ConcreteGenerics = std::vector<TypeSignature>;
