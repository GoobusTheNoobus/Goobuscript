#pragma once
#include "ast.h"
#include "value.h"
#include <memory>

class Evaluator {
    public:
    Evaluator();
    Value run(SyntaxTree& tree);

    private:
    Value evaluate(std::unique_ptr<ASTNode>& node);

    Value evaluate_binary(BinaryOperation& node);
    Value evaluate_unary(UnaryOperation& node);
};
