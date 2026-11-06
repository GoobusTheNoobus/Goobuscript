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
    for (auto &node : tree) {
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
        case ASTNodeKind::Number:
            return static_cast<Number &>(*node).value;
        case ASTNodeKind::String:
            return static_cast<String &>(*node).value;
        case ASTNodeKind::Boolean:
            return static_cast<Boolean &>(*node).value;
        case ASTNodeKind::Identifier:
            diagnostic::report_error("Identifiers are not supported");
            return Error;
        case ASTNodeKind::BinaryOperation:
            return evaluate_binary(static_cast<BinaryOperation &>(*node));
        case ASTNodeKind::UnaryOperation:
            return evaluate_unary(static_cast<UnaryOperation &>(*node));
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
            return node.operator_ == parse::TokenKind::Plus ? *value.get_num() : -(*value.get_num());
        }

        default: {
            diagnostic::report_error("Unary " + parse::token_kind_to_string(node.operator_) +
                                     " is not supported for type '" + type_to_string(value.type) + "'");
            return Error;
        }
    }
}

Value Evaluator::evaluate_binary(BinaryOperation &node) {

    if (parse::is_logical_op(node.operator_))
        return evaluate_logical(node);

    Value lhs = evaluate(node.operand1);

    if (!lhs) {
        return lhs;
    }

    Value rhs = evaluate(node.operand2);

    if (!rhs) {
        return Error;
    }

    if (parse::is_comparison_op(node.operator_))
        return evaluate_comparison(node.operator_, lhs, rhs);

    return evaluate_arithmetic(node.operator_, lhs, rhs);
}

Value Evaluator::evaluate_logical(BinaryOperation &node) {

    Value value1 = evaluate(node.operand1);
    if (!value1) {
        return value1;
    }

    switch (node.operator_) {

        // && operator: returns the first untruthy value, or the last one
        // if both are truthy
        case parse::TokenKind::AndAnd: {
            bool truthiness1 = value1.is_truthy();

            if (!truthiness1)
                return value1;

            Value value2 = evaluate(node.operand2);

            // it doesn't matter if its truthy, if its false, it is the first one to be false, so that gets
            // returned. If its true, it still gets returned, since its the last one
            return value2;
        }

        // || operator: returns the first truthy operand, or the last one
        // if both is false
        case parse::TokenKind::OrOr: {
            bool truthiness1 = value1.is_truthy();

            if (truthiness1)
                return value1;

            Value value2 = evaluate(node.operand2);

            // like the last one, it doesn't really matter if this one is
            // truthy or falsy, as both return the last one
            return value2;
        }

        default:
            throw std::runtime_error("Unknown operator " + parse::token_kind_to_string(node.operator_));
    }
}

Value Evaluator::evaluate_arithmetic(parse::TokenKind &operator_, Value &lhs, Value &rhs) {
    bool is_both_str = lhs.type == Type::String && rhs.type == Type::String;
    bool is_both_num = lhs.type == Type::Number && rhs.type == Type::Number;

    if (is_both_str && operator_ == parse::TokenKind::Plus) {

        std::string str1 = lhs.to_string();
        std::string str2 = rhs.to_string();

        return str1 + str2;
    }

    if (is_both_num) {

        double num1 = *lhs.get_num();
        double num2 = *rhs.get_num();

        switch (operator_) {
            case parse::TokenKind::Plus:
                return num1 + num2;
            case parse::TokenKind::Minus:
                return num1 - num2;
            case parse::TokenKind::Star:
                return num1 * num2;
            case parse::TokenKind::Slash:
                return num1 / num2;
            default:
                throw std::runtime_error("Unknown operator " + parse::token_kind_to_string(operator_));
        }
    }

    diagnostic::report_error("Binary " + parse::token_kind_to_string(operator_) + " is not supported for types '" +
                             type_to_string(lhs.type) + "' and '" + type_to_string(rhs.type) + "'");
    return Error;
}

Value Evaluator::evaluate_comparison(parse::TokenKind &operator_, Value &lhs, Value &rhs) {
    switch (operator_) {
        case parse::TokenKind::EqualEqual:
        case parse::TokenKind::BangEqual: {
            bool is_equal = false;

            if (lhs.type == rhs.type) {
                switch (lhs.type) {
                    case Type::Boolean:
                        is_equal = *lhs.get_bool() == *rhs.get_bool();
                        break;
                    case Type::Number:
                        is_equal = *lhs.get_num() == *rhs.get_num();
                        break;
                    case Type::String:
                        is_equal = *lhs.get_str() == *rhs.get_str();
                        break;
                    case Type::Void:
                        is_equal = true; // nothing is always nothing :P
                        break;
                    default:
                        diagnostic::report_error("Binary " + parse::token_kind_to_string(operator_) +
                                                 " is not supported for types '" + type_to_string(lhs.type) +
                                                 "' and '" + type_to_string(rhs.type) + "'");
                        return Error;
                }
            }

            return operator_ == parse::TokenKind::EqualEqual ? is_equal : !is_equal;
        }

        case parse::TokenKind::LessThan:
        case parse::TokenKind::LessEqualThan:
        case parse::TokenKind::MoreEqualThan:
        case parse::TokenKind::MoreThan: {

            if (!(lhs.type == Type::Number && rhs.type == Type::Number)) {
                diagnostic::report_error("Binary " + parse::token_kind_to_string(operator_) +
                                         " is not supported for types '" + type_to_string(lhs.type) + "' and '" +
                                         type_to_string(rhs.type) + "'");
                return Error;
            }

            double lhs_num = *lhs.get_num();
            double rhs_num = *rhs.get_num();

            switch (operator_) {
                case parse::TokenKind::LessThan:
                    return lhs_num < rhs_num;
                case parse::TokenKind::LessEqualThan:
                    return lhs_num <= rhs_num;
                case parse::TokenKind::MoreThan:
                    return lhs_num > rhs_num;
                case parse::TokenKind::MoreEqualThan:
                    return lhs_num >= rhs_num;
                default:
                    throw std::runtime_error("Unknown comparison operator");
            }

            return false;
        }

        default:
            throw std::runtime_error("Unknown operator " + parse::token_kind_to_string(operator_));
    }
}