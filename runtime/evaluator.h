#pragma once
#include "ast.h"
#include "token.h"
#include "value.h"
#include <memory>

class Evaluator {
    public:
    Evaluator();
    Value run(SyntaxTree &tree);

    private:
    Value evaluate(std::unique_ptr<ASTNode> &node);

    Value evaluate_binary(BinaryOperation &node);
    Value evaluate_unary(UnaryOperation &node);

    Value evaluate_logical(BinaryOperation &node);
    Value evaluate_comparison(parse::TokenKind &operator_, Value &lhs, Value &rhs);
    Value evaluate_arithmetic(parse::TokenKind &operator_, Value &lhs, Value &rhs);
};
