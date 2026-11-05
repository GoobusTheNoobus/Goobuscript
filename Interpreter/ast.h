#pragma once
#include "token.h"
#include <memory>
#include <ostream>
#include <string>
#include <vector>

enum class ASTNodeKind {
    Identifier,
    Number,
    String,
    Boolean,

    BinaryOperation,
    UnaryOperation,

    Invalid,
};

struct ASTNode {
    ASTNode() = default;
    virtual ~ASTNode() = default;
    virtual ASTNodeKind kind() const = 0;
    virtual std::string to_string(int indent) const = 0;
};

using SyntaxTree = std::vector<std::unique_ptr<ASTNode>>;
std::ostream& operator <<(std::ostream& out, const SyntaxTree& tree);

struct Identifier: ASTNode {
    std::string name;

    Identifier(std::string name) : name(std::move(name)) {}
    ASTNodeKind kind() const override { return ASTNodeKind::Identifier; }
    std::string to_string(int indent) const override;
};

struct String: ASTNode {
    std::string value;

    String(std::string str) : value(std::move(str)) {}
    ASTNodeKind kind() const override { return ASTNodeKind::String; }
    std::string to_string(int indent) const override;
};

struct Number: ASTNode {
    double value;

    Number(double value) : value(value) {}
    ASTNodeKind kind() const override { return ASTNodeKind::Number; }
    std::string to_string(int indent) const override;
};

struct Boolean: ASTNode {
    bool value;

    Boolean(bool value) : value(value) {}
    ASTNodeKind kind() const override { return ASTNodeKind::Boolean; }
    std::string to_string(int indent) const override;
};

struct BinaryOperation: ASTNode {
    std::unique_ptr<ASTNode> operand1, operand2;
    parse::TokenKind operator_;

    BinaryOperation(
        std::unique_ptr<ASTNode> operand1, 
        std::unique_ptr<ASTNode> operand2, 
        parse::TokenKind operator_
    ) : operand1(std::move(operand1)), operand2(std::move(operand2)), operator_(operator_) {}

    ASTNodeKind kind() const override { return ASTNodeKind::BinaryOperation; }
    std::string to_string(int indent) const override;
};

struct UnaryOperation: ASTNode {
    std::unique_ptr<ASTNode> operand;
    parse::TokenKind operator_;

    UnaryOperation(
        std::unique_ptr<ASTNode> operand, 
        parse::TokenKind operator_
    ) : operand(std::move(operand)), operator_(operator_) {}

    ASTNodeKind kind() const override { return ASTNodeKind::UnaryOperation; }
    std::string to_string(int indent) const override;
};

struct Invalid: ASTNode {
    Invalid() = default;
    ASTNodeKind kind() const override { return ASTNodeKind::Invalid; }
    std::string to_string(int indent) const override;
};