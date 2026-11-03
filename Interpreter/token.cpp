#include "token.h"

namespace parse {

std::string token_kind_to_string(TokenKind kind) {

    constexpr const char* MAP[] = {
        "Identifier",
        "Number",
        "String",

        "Plus",
        "Minus",
        "Star",
        "Slash",

        "Equal",
        "LeftParen", 
        "RightParen",

        "Semicolon", 
        "Comma",
        "EndOfFile",
    };

    return MAP[static_cast<int>(kind)];
}

}