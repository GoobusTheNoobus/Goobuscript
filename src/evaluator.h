#pragma once
#include "ast.h"
#include "token.h"
#include "value.h"
#include <memory>

class Evaluator {
    public:
    Evaluator();
    Value Run(SyntaxTree &tree);

    private:
    Value Evaluate(std::unique_ptr<ASTNode> &node);

    Value EvaluateBinary(BinaryOperation &node);
    Value EvaluateUnary(UnaryOperation &node);

    Value EvaluateLogical(BinaryOperation &node);
    Value EvaluateComparison(parse::TokenKind &operator_, Value &lhs, Value &rhs);
    Value EvaluateArithmetic(parse::TokenKind &operator_, Value &lhs, Value &rhs);
};
