#include "evaluator.h"
#include "ast.h"
#include "diagnostic.h"
#include "token.h"
#include "value.h"
#include <stdexcept>
#include <string>

Evaluator::Evaluator() = default;

Value Evaluator::Run(SyntaxTree &tree) {

    // We store the last value that was evaluated.
    Value last = Value();
    for (auto &node : tree) {
        Value value = Evaluate(node);

        if (!value) {
            return Error;
        }

        last = value;
    }

    // If the caller is the REPL, it will print this value
    return last;
}

Value Evaluator::Evaluate(std::unique_ptr<ASTNode> &node) {

    switch (node->Kind()) {
        case ASTNodeKind::Number:
            return static_cast<Number &>(*node).value;
        case ASTNodeKind::String:
            return static_cast<String &>(*node).value;
        case ASTNodeKind::Boolean:
            return static_cast<Boolean &>(*node).value;
        case ASTNodeKind::Identifier:
            diagnostic::ReportError("Identifiers are not supported");
            return Error;
        case ASTNodeKind::BinaryOperation:
            return EvaluateBinary(static_cast<BinaryOperation &>(*node));
        case ASTNodeKind::UnaryOperation:
            return EvaluateUnary(static_cast<UnaryOperation &>(*node));
        default: {
            throw std::runtime_error("Unknown ast node type id=" + std::to_string(static_cast<int>(node->Kind())));
        }
    }
}

Value Evaluator::EvaluateUnary(UnaryOperation &node) {

    Value value = Evaluate(node.operand);

    if (node.operator_ == parse::TokenKind::Bang) {
        return !value.IsTruthy();
    }

    switch (value.type) {
        case Type::Number: {
            return node.operator_ == parse::TokenKind::Plus ? *value.GetNum() : -(*value.GetNum());
        }

        default: {
            diagnostic::ReportError("Unary " + parse::TokenKindToString(node.operator_) +
                                     " is not supported for type '" + TypeToString(value.type) + "'");
            return Error;
        }
    }
}

Value Evaluator::EvaluateBinary(BinaryOperation &node) {

    if (parse::IsLogicalOp(node.operator_))
        return EvaluateLogical(node);

    Value lhs = Evaluate(node.operand1);

    if (!lhs) {
        return lhs;
    }

    Value rhs = Evaluate(node.operand2);

    if (!rhs) {
        return Error;
    }

    if (parse::IsComparisonOp(node.operator_))
        return EvaluateComparison(node.operator_, lhs, rhs);

    return EvaluateArithmetic(node.operator_, lhs, rhs);
}

Value Evaluator::EvaluateLogical(BinaryOperation &node) {

    Value value1 = Evaluate(node.operand1);
    if (!value1) {
        return value1;
    }

    switch (node.operator_) {

        // && operator: returns the first untruthy value, or the last one
        // if both are truthy
        case parse::TokenKind::AndAnd: {
            bool truthiness1 = value1.IsTruthy();

            if (!truthiness1)
                return value1;

            Value value2 = Evaluate(node.operand2);

            // it doesn't matter if its truthy, if its false, it is the first one to be false, so that gets
            // returned. If its true, it still gets returned, since its the last one
            return value2;
        }

        // || operator: returns the first truthy operand, or the last one
        // if both is false
        case parse::TokenKind::OrOr: {
            bool truthiness1 = value1.IsTruthy();

            if (truthiness1)
                return value1;

            Value value2 = Evaluate(node.operand2);

            // like the last one, it doesn't really matter if this one is
            // truthy or falsy, as both return the last one
            return value2;
        }

        default:
            throw std::runtime_error("Unknown operator " + parse::TokenKindToString(node.operator_));
    }
}

Value Evaluator::EvaluateArithmetic(parse::TokenKind &operator_, Value &lhs, Value &rhs) {
    bool is_both_str = lhs.type == Type::String && rhs.type == Type::String;
    bool is_both_num = lhs.type == Type::Number && rhs.type == Type::Number;

    if (is_both_str && operator_ == parse::TokenKind::Plus) {

        std::string str1 = lhs.ToString();
        std::string str2 = rhs.ToString();

        return str1 + str2;
    }

    if (is_both_num) {

        double num1 = *lhs.GetNum();
        double num2 = *rhs.GetNum();

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
                throw std::runtime_error("Unknown operator " + parse::TokenKindToString(operator_));
        }
    }

    diagnostic::ReportError("Binary " + parse::TokenKindToString(operator_) + " is not supported for types '" +
                             TypeToString(lhs.type) + "' and '" + TypeToString(rhs.type) + "'");
    return Error;
}

Value Evaluator::EvaluateComparison(parse::TokenKind &operator_, Value &lhs, Value &rhs) {
    switch (operator_) {
        case parse::TokenKind::EqualEqual:
        case parse::TokenKind::BangEqual: {
            bool is_equal = false;

            if (lhs.type == rhs.type) {
                switch (lhs.type) {
                    case Type::Boolean:
                        is_equal = *lhs.GetBool() == *rhs.GetBool();
                        break;
                    case Type::Number:
                        is_equal = *lhs.GetNum() == *rhs.GetNum();
                        break;
                    case Type::String:
                        is_equal = *lhs.GetStr() == *rhs.GetStr();
                        break;
                    case Type::Void:
                        is_equal = true; // nothing is always nothing :P
                        break;
                    default:
                        diagnostic::ReportError("Binary " + parse::TokenKindToString(operator_) +
                                                 " is not supported for types '" + TypeToString(lhs.type) +
                                                 "' and '" + TypeToString(rhs.type) + "'");
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
                diagnostic::ReportError("Binary " + parse::TokenKindToString(operator_) +
                                         " is not supported for types '" + TypeToString(lhs.type) + "' and '" +
                                         TypeToString(rhs.type) + "'");
                return Error;
            }

            double lhs_num = *lhs.GetNum();
            double rhs_num = *rhs.GetNum();

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
            throw std::runtime_error("Unknown operator " + parse::TokenKindToString(operator_));
    }
}