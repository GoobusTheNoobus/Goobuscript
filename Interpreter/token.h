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

    Semicolon, 
    Comma,
    EndOfFile,
};

std::string token_kind_to_string(TokenKind kind);

struct Token {
    std::string str;
    TokenKind kind;

    std::string repr() const {
        return kind == TokenKind::EndOfFile ? "<EOF>" : "'" + str + "'";
    }

    std::string to_string() const {
        return token_kind_to_string(kind) + "('" + str + "')";
    }
};

inline std::ostream& operator <<(std::ostream& out, const Token& token) {
    out << token.to_string();
    return out;
}

inline std::ostream& operator <<(std::ostream& out, const std::vector<Token>& tokens) {
    for (const auto& token : tokens) {
        out << token << '\n';
    }

    return out;
}

}