#include "parser.h"
#include "ast.h"
#include "diagnostic.h"
#include "token.h"
#include <memory>
#include <string>
#include <format>

namespace parse {

SyntaxTree Parser::parse() {

    SyntaxTree tree;

    while (!end()) {
        if (peek().kind == TokenKind::EndOfFile) {
            break;
        }

        tree.push_back(parse_expression());
    }

    return tree;
}

std::unique_ptr<ASTNode> Parser::parse_expression() {
    return parse_additive();
}

std::unique_ptr<ASTNode> Parser::parse_additive() {
    auto lhs = parse_multiplicative();

    while (!end() && (peek().kind == TokenKind::Plus || peek().kind == TokenKind::Minus)) {
        TokenKind operator_ = next().kind;

        auto rhs = parse_multiplicative();
        lhs = std::make_unique<BinaryOperation>(std::move(lhs), std::move(rhs), operator_);
    }

    return lhs;
}

std::unique_ptr<ASTNode> Parser::parse_multiplicative() {
    auto lhs = parse_unary();

    while (!end() && (peek().kind == TokenKind::Star || peek().kind == TokenKind::Slash)) {
        TokenKind operator_ = next().kind;

        auto rhs = parse_unary();
        lhs = std::make_unique<BinaryOperation>(std::move(lhs), std::move(rhs), operator_);
    }

    return lhs;
}

std::unique_ptr<ASTNode> Parser::parse_unary() {

    if (!end() && (peek().kind == TokenKind::Plus || peek().kind == TokenKind::Minus)) {
        TokenKind operator_ = next().kind;

        auto operand = parse_unary();
        return std::make_unique<UnaryOperation>(std::move(operand), operator_);
    }

    return parse_primary();
}

std::unique_ptr<ASTNode> Parser::parse_primary() {

    switch (peek().kind) {
        case parse::TokenKind::Number: {
            return std::make_unique<Number>(std::stof(next().str));
        }

        case parse::TokenKind::Identifier: {
            return std::make_unique<Identifier>(next().str);
        }

        case parse::TokenKind::String: {
            return std::make_unique<String>(next().str);
        }

        case parse::TokenKind::True: 
        case parse::TokenKind::False: {
            return std::make_unique<Boolean>(next().str == "true");
        }

        case parse::TokenKind::LeftParen: {
            next();

            auto expression = parse_expression();

            if (peek().kind != TokenKind::RightParen) {
                diagnostic::report_error(std::format("Expected ')', got {}", peek().repr()));
            }
            else next();

            return expression;
        }

        default: {
            diagnostic::report_error(std::format("Expected expression, got {}", next().repr()));
            return std::make_unique<Invalid>();
        }
    }
}


const Token& Parser::peek() const {
    if (pos < tokens.size()) {
        return tokens[pos];
    }

    return tokens.back();
}

const Token& Parser::peek(ptrdiff_t offset) const {

    auto lookup = static_cast<ptrdiff_t>(pos) + offset;

    if (lookup < 0 || lookup >= tokens.size()) {
        return tokens.back();
    }

    return tokens[lookup];
}

bool Parser::end() const {
    return pos >= tokens.size();
}

const Token& Parser::next()
{
    if (end())
        return tokens.back();

    return tokens[pos++];
}

void Parser::next(size_t amount) {
    pos += amount;
}

}
