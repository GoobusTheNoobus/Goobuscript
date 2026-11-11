#pragma once
#include "token.h"
#include <cstddef>
#include <string>
#include <vector>

namespace parse {

class Lexer {
    public:
    Lexer(std::string source) : source(std::move(source)) {
    }

    std::vector<Token> Tokenize();

    private:
    const std::string source;
    size_t pos = 0;

    bool TryTokenizeSymbol(std::vector<Token> *tokens);
    bool TryTokenizeWord(std::vector<Token> *tokens);
    bool TryTokenizeNumber(std::vector<Token> *tokens);
    bool TryTokenizeString(std::vector<Token> *tokens);

    char Peek() const;
    char Peek(ptrdiff_t offset) const;

    bool End() const;

    char Next();
    void Next(size_t amount); // should never go back

    char GetEscape();
};

} // namespace parse