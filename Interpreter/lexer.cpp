#include "lexer.h"
#include "core.h"
#include "diagnostic.h"
#include "token.h"
#include <cctype>
#include <cstddef>
#include <string>
#include <vector>

namespace parse {

std::vector<Token> Lexer::tokenize() {

    std::vector<Token> tokens;

    while (!end()) {

        char cur = peek();

        if (std::isspace(cur)) {
            next();
            continue;
        }

        if (try_tokenize_symbol(&tokens)) {
            continue;
        }

        if (try_tokenize_identifier(&tokens)) {
            continue;
        }

        if (try_tokenize_number(&tokens)) {
            continue;
        }

        if (try_tokenize_string(&tokens)) {
            continue;
        }

        diagnostic::report_error("Unexpected character '" + std::string(1, next()) + "'");
        
    }

    tokens.push_back({"", TokenKind::EndOfFile});
    return tokens;
}

bool Lexer::try_tokenize_symbol(std::vector<Token> *tokens) {

    switch (peek()) {
        case '(': 
            tokens->push_back({"(", TokenKind::LeftParen});
            next();
            return true;
        case ')':
            tokens->push_back({")", TokenKind::RightParen});
            next();
            return true;
        case ';':
            tokens->push_back({";", TokenKind::Semicolon});
            next();
            return true;
        case ',':
            tokens->push_back({",", TokenKind::Comma});
            next();
            return true;
        case '=':
            tokens->push_back({"=", TokenKind::Equal});
            next();
            return true;
        case '+':
            tokens->push_back({"+", TokenKind::Plus});
            next();
            return true;
        case '-':
            tokens->push_back({"-", TokenKind::Minus});
            next();
            return true;
        case '*':
            tokens->push_back({"*", TokenKind::Star});
            next();
            return true;
        case '/':
            tokens->push_back({"/", TokenKind::Slash});
            next();
            return true;
        default:
            return false;
    }
}

bool Lexer::try_tokenize_identifier(std::vector<Token> *tokens) {
    if (!is_identifier_start(peek())) {
        return false;
    }

    std::string str;

    while (!end() && is_identifier_continue(peek())) {
        str += next();
    }

    tokens->push_back({str, TokenKind::Identifier});
    return true;
}

bool Lexer::try_tokenize_number(std::vector<Token> *tokens) {
    if (!is_number_start(peek())) {
        return false;
    }

    std::string str;
    int dots_encountered = 0;

    while (!end() && is_number_continue(peek())) {

        if (peek() == '.') {
            if (dots_encountered == 1) {
                diagnostic::report_error("Too many dots in number literal");
            }

            dots_encountered++;
        }

        str += next();

    }

    tokens->push_back({str, TokenKind::Number});
    return true;
}

bool Lexer::try_tokenize_string(std::vector<Token> *tokens) {
    if (peek() != '"' && peek() != '\'') {
        return false;
    }

    char quote = next();
    std::string str;

    while (!end() && peek() != '\n' && peek() != quote) {
        
        if (peek() == '\\') {
            next(); // skip the backslash
            
            if (end() || peek() == '\n') {
                diagnostic::report_error("Unterminated escape sequence");
                break;
            }

            str += get_escape();
            continue;
        }

        str += next();
    }

    if (peek() != quote) {
        diagnostic::report_error("Unterminated string literal");
    } else next();

    tokens->push_back({str, TokenKind::String});
    return true;
}

char Lexer::get_escape() {
    switch (next()) {
        case '\\': return '\\';
        case '\'': return '\'';
        case '"': return '"';
        case 'n': return '\n';
        case 'r': return '\r';
        case 't': return '\t';
        case 'v': return '\v';
        case 'e': return '\e';
        default: {
            diagnostic::report_error("Unknown escape sequence");
            return ' ';
        }
    }
}
    
char Lexer::peek() const {
    if (pos < source.size()) {
        return source[pos];
    }

    return '\0';
}

char Lexer::peek(ptrdiff_t offset) const {

    auto lookup = static_cast<ptrdiff_t>(pos) + offset;

    if (lookup < 0 || lookup >= source.size()) {
        return '\0';
    }

    return source[lookup];
}

bool Lexer::end() const {
    return peek() == '\0';
}

char Lexer::next() {
    char c = peek();
    ++pos;
    return c;
}

void Lexer::next(size_t amount) {
    pos += amount;
}

}