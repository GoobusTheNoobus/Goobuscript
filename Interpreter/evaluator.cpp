#include "evaluator.h"
#include "ast.h"
#include "diagnostic.h"
#include "token.h"
#include "value.h"
#include <stdexcept>
#include <string>

Evaluator::Evaluator() = default;

Value Evaluator::run(SyntaxTree &tree) {

    // We store the last value that was evaluated.
    Value last = Value();
    for (auto& node : tree) {
        Value value = evaluate(node);

        if (!value) {
            return Error;
        }

        last = value;
    }

    // If the caller is the REPL, it will print this value
    return last;
}

Value Evaluator::evaluate(std::unique_ptr<ASTNode> &node) {

    switch (node->kind()) {
        case ASTNodeKind::Number: return static_cast<Number&>(*node).value;
        case ASTNodeKind::String: return static_cast<String&>(*node).value;
        case ASTNodeKind::Boolean: return static_cast<Boolean&>(*node).value;
        case ASTNodeKind::Identifier: diagnostic::report_error("Identifiers are not supported"); return Error;
        case ASTNodeKind::BinaryOperation: return evaluate_binary(static_cast<BinaryOperation&>(*node));
        case ASTNodeKind::UnaryOperation: return evaluate_unary(static_cast<UnaryOperation&>(*node));
        default: {
            throw std::runtime_error("Unknown ast node type id=" + std::to_string(static_cast<int>(node->kind())));
        }
    }
}

Value Evaluator::evaluate_unary(UnaryOperation &node) {

    Value value = evaluate(node.operand);

    if (node.operator_ == parse::TokenKind::Bang) {
        return !value.is_truthy();
    }

    switch (value.type) {
        case Type::Number: {
            return node.operator_ == parse::TokenKind::Plus ? 
                   *value.get_num() : -(*value.get_num());
        }

        default: {
            diagnostic::report_error(
                "Unary " + parse::token_kind_to_string(node.operator_) + " is not supported for type '" + 
                type_to_string(value.type) + "'");
            return Error;
        }
    }
}

Value Evaluator::evaluate_binary(BinaryOperation &node) {

    bool is_logical = node.operator_ == parse::TokenKind::AndAnd || 
                      node.operator_ == parse::TokenKind::OrOr;

    Value value1 = evaluate(node.operand1);

    // Javascript has some unusual logical operator handling. Instead of 
    // comparing booleans, truthiness is the factor that gets decided.

    if (is_logical) {
        switch (node.operator_) {

            // && operator: returns the first untruthy value, or the last one
            // if both are truthy
            case parse::TokenKind::AndAnd: {
                bool truthiness1 = value1.is_truthy();
                
                if (!truthiness1) return value1; 

                Value value2 = evaluate(node.operand2);

                // it doesn't matter if its truthy, if its false, it is the first one 
                // to be false, so that gets returned. If its true, it still gets returned,
                // since its the last one
                return value2;
            }

            // || operator: returns the first truthy operand, or the last one
            // if both is false
            case parse::TokenKind::OrOr: {
                bool truthiness1 = value1.is_truthy();

                if (truthiness1) return value1;

                Value value2 = evaluate(node.operand2);

                // like the last one, it doesn't really matter if this one is truthy or falsy,
                // as both return the last one
                return value2;
            }

            default: throw std::runtime_error("Unknown operator " + 
                parse::token_kind_to_string(node.operator_));
        }
    }

    Value value2 = evaluate(node.operand2);

    // Propagate error
    if (!value1 || !value2) {
        return Error;
    }

    bool is_both_str = value1.type == Type::String && value2.type == Type::String;
    bool is_both_num = value1.type == Type::Number && value2.type == Type::Number;
    

    if (is_both_str && node.operator_ == parse::TokenKind::Plus) {

        // concat string

        std::string str1 = value1.to_string();
        std::string str2 = value2.to_string();

        return str1 + str2;
    }

    if (is_both_num) {

        double num1 = *value1.get_num();
        double num2 = *value2.get_num();

        switch (node.operator_) {
            case parse::TokenKind::Plus: return num1 + num2;
            case parse::TokenKind::Minus: return num1 - num2;
            case parse::TokenKind::Star: return num1 * num2;
            case parse::TokenKind::Slash: return num1 / num2;
            default: throw std::runtime_error("Unknown operator " + 
                parse::token_kind_to_string(node.operator_));
        }
    }

    diagnostic::report_error(
        "Binary " + parse::token_kind_to_string(node.operator_) + " is not supported for types '" + 
        type_to_string(value1.type) + "' and '" + type_to_string(value2.type) + "'");
    return Error;
}