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

    using RecursiveAdoption = std::pair<std::string, CommonFeatureAdoptionPlanDescriptor>;

    std::vector<RecursiveAdoption> recursive_adoptions;
    FunctionDefinition::Ref direct_adoption;
};