#pragma once
#include "token.h"
#include <cstddef>
#include <string>
#include <vector>

namespace parse {

class Lexer {
    public:
    Lexer(std::string source) : source(std::move(source)) {}

    std::vector<Token> tokenize();

    private:
    const std::string source;
    size_t pos = 0;
    
    bool try_tokenize_symbol(std::vector<Token>* tokens);
    bool try_tokenize_word(std::vector<Token>* tokens);
    bool try_tokenize_number(std::vector<Token>* tokens);
    bool try_tokenize_string(std::vector<Token>* tokens);

    char peek() const;
    char peek(ptrdiff_t offset) const;

    bool end() const;
    
    char next();
    void next(size_t amount); // should never go back

    char get_escape();
    
};

}