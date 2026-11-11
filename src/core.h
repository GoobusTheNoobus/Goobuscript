#pragma once
#include <cctype>
#include <cstddef>
#include <string>
#define VERSION "0.1.0"

inline void StringStrip(std::string *str) {
    const char WHITESPACE[] = " \t\n\r\f\v";

    size_t start = str->find_first_not_of(WHITESPACE);
    if (start != std::string::npos) {
        str->erase(0, start);
    } else {
        str->clear();
        return;
    }

    size_t end = str->find_last_not_of(WHITESPACE);

    if (end != std::string::npos) {
        str->erase(end + 1);
    }
}

inline bool IsIdentifierStart(char c) {
    return c == '_' || std::isalpha(static_cast<unsigned char>(c));
}

inline bool IsIdentifierContinue(char c) {
    return IsIdentifierStart(c) ||
           std::isdigit(static_cast<unsigned char>(c));
}

inline bool IsNumberStart(char c) {
    return std::isdigit(static_cast<unsigned char>(c));
}

inline bool IsNumberContinue(char c) {
    return IsNumberStart(c) || c == '.';
}