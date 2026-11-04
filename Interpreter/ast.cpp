#include "ast.h"
#include "token.h"
#include <string>

std::string Identifier::to_string(int indent) const {
    return std::string(indent, ' ') + "Identifier(" + name + ")\n";
}

std::string String::to_string(int indent) const {
    return std::string(indent, ' ') + "String('" + value + "')\n";
}

std::string Number::to_string(int indent) const {
    return std::string(indent, ' ') + "Number(" + std::to_string(value) + ")\n";
}

std::string BinaryOperation::to_string(int indent) const {
    return 
        std::string(indent, ' ') + "BinaryOperation(" + parse::token_kind_to_string(operator_) + ")\n" +
        operand1->to_string(indent + 2) + operand2->to_string(indent + 2);
}

std::string UnaryOperation::to_string(int indent) const {
    return 
        std::string(indent, ' ') + "UnaryOperation(" + parse::token_kind_to_string(operator_) + ")\n" +
        operand->to_string(indent + 2);
}

std::string Invalid::to_string(int indent) const {
    return std::string(indent, ' ') + "Invalid()\n";
}

std::ostream& operator <<(std::ostream& out, const SyntaxTree& tree) {
    for (const auto& node : tree) {
        out << node->to_string(0);
    }

    return out;
}