#pragma once
#include "ast.h"
#include "token.h"
#include <memory>
#include <vector>

namespace parse {

class Parser {
    public:
    Parser(std::vector<Token> tokens) : tokens(std::move(tokens)) {
    }
    SyntaxTree parse();

    private:
    const std::vector<Token> tokens;
    size_t pos = 0;

    std::unique_ptr<ASTNode> ParseExpression();

    std::unique_ptr<ASTNode> ParseLogicalOr();
    std::unique_ptr<ASTNode> ParseLogicalAnd();

    std::unique_ptr<ASTNode> ParseEquality();
    std::unique_ptr<ASTNode> ParseRelational();

    std::unique_ptr<ASTNode> ParseAdditive();
    std::unique_ptr<ASTNode> ParseMultiplicative();
    std::unique_ptr<ASTNode> ParseUnary();
    std::unique_ptr<ASTNode> ParsePrimary();

    const Token &Peek() const;
    const Token &Peek(ptrdiff_t offset) const;

    bool End() const;

    const Token &Next();
    void Next(size_t amount); // should never go back
};

} // namespace parse