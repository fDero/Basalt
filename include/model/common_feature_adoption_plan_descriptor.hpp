//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <memory>
#include <string>
#include <map>

#include "language/definitions.hpp"

struct CommonFeatureAdoptionPlanDescriptor {
    
    using DirectAdoption = std::pair<std::string, FunctionDefinition::Ref>;
    using RecursiveAdoption = std::pair<std::string, CommonFeatureAdoptionPlanDescriptor>;

    std::vector<RecursiveAdoption> recursive_adoptions;
    std::vector<DirectAdoption> direct_adoptions;
};