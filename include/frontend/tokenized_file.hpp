
#pragma once

#include <string>
#include <vector>

#include "frontend/token.hpp"

struct TokenizedFile {
    std::string filename;
    std::vector<Token> tokens;
};