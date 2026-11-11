#include "lexer.h"
#include "core.h"
#include "diagnostic.h"
#include "token.h"
#include <cctype>
#include <cstddef>
#include <string>
#include <vector>

namespace parse {

std::vector<Token> Lexer::Tokenize() {

    std::vector<Token> tokens;

    while (!End()) {

        char cur = Peek();

        if (std::isspace(cur)) {
            Next();
            continue;
        }

        if (TryTokenizeSymbol(&tokens)) {
            continue;
        }

        if (TryTokenizeWord(&tokens)) {
            continue;
        }

        if (TryTokenizeNumber(&tokens)) {
            continue;
        }

        if (TryTokenizeString(&tokens)) {
            continue;
        }

        diagnostic::ReportError("Unexpected character '" +
                                 std::string(1, Next()) + "'");
    }

    tokens.push_back({"", TokenKind::EndOfFile});
    return tokens;
}

bool Lexer::TryTokenizeSymbol(std::vector<Token> *tokens) {

    switch (Peek()) {
        case '(':
            tokens->push_back({"(", TokenKind::LeftParen});
            Next();
            return true;
        case ')':
            tokens->push_back({")", TokenKind::RightParen});
            Next();
            return true;
        case ';':
            tokens->push_back({";", TokenKind::Semicolon});
            Next();
            return true;
        case ',':
            tokens->push_back({",", TokenKind::Comma});
            Next();
            return true;
        case '+':
            tokens->push_back({"+", TokenKind::Plus});
            Next();
            return true;
        case '-':
            tokens->push_back({"-", TokenKind::Minus});
            Next();
            return true;
        case '*':
            tokens->push_back({"*", TokenKind::Star});
            Next();
            return true;
        case '/':
            tokens->push_back({"/", TokenKind::Slash});
            Next();
            return true;
        case '&':
            if (Peek(1) == '&') {
                tokens->push_back({"&&", TokenKind::AndAnd});
                Next(2);
                return true;
            }

            return false;
        case '|':
            if (Peek(1) == '|') {
                tokens->push_back({"||", TokenKind::OrOr});
                Next(2);
                return true;
            }

            return false;
        case '<':
            if (Peek(1) == '=') {
                tokens->push_back({"<=", TokenKind::LessEqualThan});
                Next(2);
                return true;
            }

            tokens->push_back({"<", TokenKind::LessThan});
            Next();
            return true;
        case '>':
            if (Peek(1) == '=') {
                tokens->push_back({">=", TokenKind::MoreEqualThan});
                Next(2);
                return true;
            }

            tokens->push_back({">", TokenKind::MoreThan});
            Next();
            return true;
        case '=':
            if (Peek(1) == '=') {
                tokens->push_back({"==", TokenKind::EqualEqual});
                Next(2);
                return true;
            }

            tokens->push_back({"=", TokenKind::Equal});
            Next();
            return true;
        case '!':
            if (Peek(1) == '=') {
                tokens->push_back({"!=", TokenKind::BangEqual});
                Next(2);
                return true;
            }

            tokens->push_back({"!", TokenKind::Bang});
            Next();
            return true;
        default:
            return false;
    }
}

bool Lexer::TryTokenizeWord(std::vector<Token> *tokens) {
    if (!IsIdentifierStart(Peek())) {
        return false;
    }

    std::string str;

    while (!End() && IsIdentifierContinue(Peek())) {
        str += Next();
    }

    auto kind = TokenKind::Identifier;
    if (str == "true")
        kind = TokenKind::True;

    if (str == "false")
        kind = TokenKind::False;

    tokens->push_back({str, kind});
    return true;
}

bool Lexer::TryTokenizeNumber(std::vector<Token> *tokens) {
    if (!IsNumberStart(Peek())) {
        return false;
    }

    std::string str;
    int dots_encountered = 0;

    while (!End() && IsNumberContinue(Peek())) {

        if (Peek() == '.') {
            if (dots_encountered == 1) {
                diagnostic::ReportError("Too many dots in number literal");
            }

            dots_encountered++;
        }

        str += Next();
    }

    tokens->push_back({str, TokenKind::Number});
    return true;
}

bool Lexer::TryTokenizeString(std::vector<Token> *tokens) {
    if (Peek() != '"' && Peek() != '\'') {
        return false;
    }

    char quote = Next();
    std::string str;

    while (!End() && Peek() != '\n' && Peek() != quote) {

        if (Peek() == '\\') {
            Next(); // skip the backslash

            if (End() || Peek() == '\n') {
                diagnostic::ReportError("Unterminated escape sequence");
                break;
            }

            str += GetEscape();
            continue;
        }

        str += Next();
    }

    if (Peek() != quote) {
        diagnostic::ReportError("Unterminated string literal");
    } else
        Next();

    tokens->push_back({str, TokenKind::String});
    return true;
}

char Lexer::GetEscape() {
    switch (Next()) {
        case '\\':
            return '\\';
        case '\'':
            return '\'';
        case '"':
            return '"';
        case 'n':
            return '\n';
        case 'r':
            return '\r';
        case 't':
            return '\t';
        case 'v':
            return '\v';
        case 'e':
            return '\e';
        default: {
            diagnostic::ReportError("Unknown escape sequence");
            return ' ';
        }
    }
}

char Lexer::Peek() const {
    if (pos < source.size()) {
        return source[pos];
    }

    return '\0';
}

char Lexer::Peek(ptrdiff_t offset) const {

    auto lookup = static_cast<ptrdiff_t>(pos) + offset;

    if (lookup < 0 || lookup >= static_cast<ptrdiff_t>(source.size())) {
        return '\0';
    }

    return source[lookup];
}

bool Lexer::End() const {
    return Peek() == '\0';
}

char Lexer::Next() {
    char c = Peek();
    ++pos;
    return c;
}

void Lexer::Next(size_t amount) {
    pos += amount;
}

} // namespace parse