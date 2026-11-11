#pragma once

#include <ostream>
#include <string>
#include <vector>
namespace parse {

enum class TokenKind {
    Identifier,
    Number,
    String,

    Plus,
    Minus,
    Star,
    Slash,

    Equal,
    LeftParen,
    RightParen,

    AndAnd,
    OrOr,
    Bang,

    LessThan,
    LessEqualThan,
    MoreThan,
    MoreEqualThan,

    EqualEqual,
    BangEqual,

    True,
    False,

    Semicolon,
    Comma,
    EndOfFile,
};

std::string TokenKindToString(TokenKind kind);
bool IsLogicalOp(TokenKind op);
bool IsComparisonOp(TokenKind op);

struct Token {
    std::string str;
    TokenKind kind;

    std::string Repr() const {
        return kind == TokenKind::EndOfFile ? "<EOF>" : "'" + str + "'";
    }

    std::string ToString() const {
        return TokenKindToString(kind) + "('" + str + "')";
    }
};

inline std::ostream &operator<<(std::ostream &out, const Token &token) {
    out << token.ToString();
    return out;
}

inline std::ostream &operator<<(std::ostream &out, const std::vector<Token> &tokens) {
    for (const auto &token : tokens) {
        out << token << '\n';
    }

    return out;
}

} // namespace parse