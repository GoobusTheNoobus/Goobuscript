#include "parser.h"
#include "ast.h"
#include "diagnostic.h"
#include "token.h"
#include <cstddef>
#include <memory>
#include <string>
#include <format>

namespace parse {

SyntaxTree Parser::parse() {

    SyntaxTree tree;

    while (!End()) {
        if (Peek().kind == TokenKind::EndOfFile) {
            break;
        }

        tree.push_back(ParseExpression());
    }

    return tree;
}

std::unique_ptr<ASTNode> Parser::ParseExpression() {
    return ParseLogicalOr();
}

std::unique_ptr<ASTNode> Parser::ParseLogicalOr() {
    auto lhs = ParseLogicalAnd();

    while (!End() && Peek().kind == TokenKind::OrOr) {
        TokenKind operator_ = Next().kind;

        auto rhs = ParseLogicalAnd();
        lhs = std::make_unique<BinaryOperation>(std::move(lhs), std::move(rhs),
                                                operator_);
    }

    return lhs;
}

std::unique_ptr<ASTNode> Parser::ParseLogicalAnd() {
    auto lhs = ParseEquality();

    while (!End() && Peek().kind == TokenKind::AndAnd) {
        TokenKind operator_ = Next().kind;

        auto rhs = ParseEquality();
        lhs = std::make_unique<BinaryOperation>(std::move(lhs), std::move(rhs),
                                                operator_);
    }

    return lhs;
}

std::unique_ptr<ASTNode> Parser::ParseEquality() {
    auto lhs = ParseRelational();

    while (!End() && (Peek().kind == TokenKind::EqualEqual ||
                      Peek().kind == TokenKind::BangEqual)) {
        TokenKind operator_ = Next().kind;

        auto rhs = ParseRelational();
        lhs = std::make_unique<BinaryOperation>(std::move(lhs), std::move(rhs),
                                                operator_);
    }

    return lhs;
}

std::unique_ptr<ASTNode> Parser::ParseRelational() {
    auto lhs = ParseAdditive();

    while (!End() && (Peek().kind == TokenKind::LessThan ||
                      Peek().kind == TokenKind::LessEqualThan ||
                      Peek().kind == TokenKind::MoreThan ||
                      Peek().kind == TokenKind::MoreEqualThan)) {
        TokenKind operator_ = Next().kind;

        auto rhs = ParseAdditive();
        lhs = std::make_unique<BinaryOperation>(std::move(lhs), std::move(rhs),
                                                operator_);
    }

    return lhs;
}

std::unique_ptr<ASTNode> Parser::ParseAdditive() {
    auto lhs = ParseMultiplicative();

    while (!End() && (Peek().kind == TokenKind::Plus ||
                      Peek().kind == TokenKind::Minus)) {
        TokenKind operator_ = Next().kind;

        auto rhs = ParseMultiplicative();
        lhs = std::make_unique<BinaryOperation>(std::move(lhs), std::move(rhs),
                                                operator_);
    }

    return lhs;
}

std::unique_ptr<ASTNode> Parser::ParseMultiplicative() {
    auto lhs = ParseUnary();

    while (!End() && (Peek().kind == TokenKind::Star ||
                      Peek().kind == TokenKind::Slash)) {
        TokenKind operator_ = Next().kind;

        auto rhs = ParseUnary();
        lhs = std::make_unique<BinaryOperation>(std::move(lhs), std::move(rhs),
                                                operator_);
    }

    return lhs;
}

std::unique_ptr<ASTNode> Parser::ParseUnary() {

    if (!End() &&
        (Peek().kind == TokenKind::Plus || Peek().kind == TokenKind::Minus)) {
        TokenKind operator_ = Next().kind;

        auto operand = ParseUnary();
        return std::make_unique<UnaryOperation>(std::move(operand), operator_);
    }

    return ParsePrimary();
}

std::unique_ptr<ASTNode> Parser::ParsePrimary() {

    switch (Peek().kind) {
        case parse::TokenKind::Number: {
            return std::make_unique<Number>(std::stof(Next().str));
        }

        case parse::TokenKind::Identifier: {
            return std::make_unique<Identifier>(Next().str);
        }

        case parse::TokenKind::String: {
            return std::make_unique<String>(Next().str);
        }

        case parse::TokenKind::True:
        case parse::TokenKind::False: {
            return std::make_unique<Boolean>(Next().str == "true");
        }

        case parse::TokenKind::LeftParen: {
            Next();

            auto expression = ParseExpression();

            if (Peek().kind != TokenKind::RightParen) {
                diagnostic::ReportError(
                    std::format("Expected ')', got {}", Peek().Repr()));
            } else
                Next();

            return expression;
        }

        default: {
            diagnostic::ReportError(
                std::format("Expected expression, got {}", Next().Repr()));
            return std::make_unique<Invalid>();
        }
    }
}

const Token &Parser::Peek() const {
    if (pos < tokens.size()) {
        return tokens[pos];
    }

    return tokens.back();
}

const Token &Parser::Peek(ptrdiff_t offset) const {

    auto lookup = static_cast<ptrdiff_t>(pos) + offset;

    if (lookup < 0 || lookup >= static_cast<ptrdiff_t>(tokens.size())) {
        return tokens.back();
    }

    return tokens[lookup];
}

bool Parser::End() const {
    return pos >= tokens.size();
}

const Token &Parser::Next() {
    if (End())
        return tokens.back();

    return tokens[pos++];
}

void Parser::Next(size_t amount) {
    pos += amount;
}

} // namespace parse
