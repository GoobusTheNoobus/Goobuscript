#pragma once
#include <iostream>
#include <string>

namespace diagnostic {

inline bool has_error = false;

inline void report_error(const std::string& str) {
    std::cout << "\033[31merror: " << str << "\033[0m\n";
    has_error = true;
}

}