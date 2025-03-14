//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once
#include <string>

[[nodiscard]] inline std::string red(const std::string& str) { 
    return std::string("\033[0;31m") + str + std::string("\033[0;0m"); 
}

[[nodiscard]] inline std::string green(const std::string& str) { 
    return std::string("\033[0;32m") + str + std::string("\033[0;0m"); 
}

[[nodiscard]] inline std::string yellow(const std::string& str) { 
    return std::string("\033[0;33m") + str + std::string("\033[0;0m"); 
}

[[nodiscard]] inline std::string blue(const std::string& str) { 
    return std::string("\033[0;34m") + str + std::string("\033[0;0m"); 
}

[[nodiscard]] inline std::string purple(const std::string& str) { 
    return std::string("\033[0;35m") + str + std::string("\033[0;0m"); 
}

[[nodiscard]] inline std::string aqua(const std::string& str) { 
    return std::string("\033[0;36m") + str + std::string("\033[0;0m"); 
}

[[nodiscard]] inline std::string gray(const std::string& str) { 
    return std::string("\033[0;37m") + str + std::string("\033[0;0m"); 
}
 
[[nodiscard]] inline std::string bold_red(const std::string& str) { 
    return std::string("\033[1;31m") + str + std::string("\033[0;0m"); 
}

[[nodiscard]] inline std::string bold_green(const std::string& str) { 
    return std::string("\033[1;32m") + str + std::string("\033[0;0m"); 
}

[[nodiscard]] inline std::string bold_yellow(const std::string& str) { 
    return std::string("\033[1;33m") + str + std::string("\033[0;0m"); 
}

[[nodiscard]] inline std::string bold_blue(const std::string& str) { 
    return std::string("\033[1;34m") + str + std::string("\033[0;0m"); 
}

[[nodiscard]] inline std::string bold_purple(const std::string& str) { 
    return std::string("\033[1;35m") + str + std::string("\033[0;0m"); 
}

[[nodiscard]] inline std::string bold_aqua(const std::string& str) { 
    return std::string("\033[1;36m") + str + std::string("\033[0;0m"); 
}

[[nodiscard]] inline std::string bold_gray(const std::string& str) { 
    return std::string("\033[1;37m") + str + std::string("\033[0;0m"); 
}