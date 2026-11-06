#pragma once
#include "ast.h"
#include "token.h"
#include <memory>
#include <vector>

namespace parse {

class Parser {
    public:
    Parser(std::vector<Token> tokens) : tokens(std::move(tokens)) {}
    SyntaxTree parse();

    private:
    const std::vector<Token> tokens;
    size_t pos = 0;

    std::unique_ptr<ASTNode> parse_expression();

    std::unique_ptr<ASTNode> parse_logical_or();
    std::unique_ptr<ASTNode> parse_logical_and();

    std::unique_ptr<ASTNode> parse_additive();
    std::unique_ptr<ASTNode> parse_multiplicative();
    std::unique_ptr<ASTNode> parse_unary();
    std::unique_ptr<ASTNode> parse_primary();

    const Token& peek() const;
    const Token& peek(ptrdiff_t offset) const;

    bool end() const;
    
    const Token& next();
    void next(size_t amount); // should never go back
};

}