#pragma once
#include <iostream>
#include <string>

namespace diagnostic {

inline bool has_error = false;

inline void ReportError(const std::string &str) {
    std::cout << "\033[31m\033[1mError\033[0m: " << str << "\n";
    has_error = true;
}

} // namespace diagnostic