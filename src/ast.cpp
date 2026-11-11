#include "ast.h"
#include "token.h"
#include <string>

std::string Identifier::ToString(int indent) const {
    return std::string(indent, ' ') + "Identifier(" + name + ")\n";
}

std::string String::ToString(int indent) const {
    return std::string(indent, ' ') + "String('" + value + "')\n";
}

std::string Number::ToString(int indent) const {
    return std::string(indent, ' ') + "Number(" + std::to_string(value) + ")\n";
}

std::string Boolean::ToString(int indent) const {
    return std::string(indent, ' ') + "Boolean(" + (value ? "true" : "false") +
         ")\n";
}

std::string BinaryOperation::ToString(int indent) const {
    return std::string(indent, ' ') + "BinaryOperation(" +
         parse::TokenKindToString(operator_) + ")\n" +
         operand1->ToString(indent + 2) + operand2->ToString(indent + 2);
}

std::string UnaryOperation::ToString(int indent) const {
    return std::string(indent, ' ') + "UnaryOperation(" +
         parse::TokenKindToString(operator_) + ")\n" +
         operand->ToString(indent + 2);
}

std::string Invalid::ToString(int indent) const {
    return std::string(indent, ' ') + "Invalid()\n";
}

std::ostream &operator<<(std::ostream &out, const SyntaxTree &tree) {
    for (const auto &node : tree) {
        out << node->ToString(0);
    }

    return out;
}