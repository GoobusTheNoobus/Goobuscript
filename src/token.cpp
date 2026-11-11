#include "token.h"

namespace parse {

std::string TokenKindToString(TokenKind kind) {

    constexpr const char *MAP[] = {
        "Identifier",    "Number",     "String",    "Plus", "Minus", "Star",      "Slash",         "Equal",
        "LeftParen",     "RightParen", "AndAnd",    "OrOr", "Bang",  "LessThan",  "LessEqualThan", "MoreThan",
        "MoreEqualThan", "EqualEqual", "BangEqual", "True", "False", "Semicolon", "Comma",         "EndOfFile",
    };

    return MAP[static_cast<int>(kind)];
}

bool IsLogicalOp(TokenKind op) {
    return op == parse::TokenKind::AndAnd || op == parse::TokenKind::OrOr || op == TokenKind::Bang;
}

bool IsComparisonOp(TokenKind op) {
    return op == parse::TokenKind::EqualEqual || op == parse::TokenKind::BangEqual ||
           op == parse::TokenKind::LessThan || op == parse::TokenKind::MoreThan ||
           op == parse::TokenKind::LessEqualThan || op == parse::TokenKind::MoreEqualThan;
}

} // namespace parse