#pragma once
#include <optional>
#include <string>
#include <variant>

enum class Type {
    Number,
    String,
    Boolean,

    Void,
    Error,
};

std::string TypeToString(Type type);

struct Value {
    std::variant<double, std::string, bool, std::monostate> data;
    Type type;

    Value(double data) : data(data), type(Type::Number) {
    }
    Value(std::string data) : data(std::move(data)), type(Type::String) {
    }
    Value(bool data) : data(data), type(Type::Boolean) {
    }
    Value() : type(Type::Void) {
    }
    Value(std::monostate) : type(Type::Error) {
    }

    operator bool() const {
        return type != Type::Error;
    }

    std::string ToString() const;

    std::optional<double> GetNum() const;
    std::optional<std::string> GetStr() const;
    std::optional<bool> GetBool() const;

    bool IsTruthy() const;
};

inline const Value Error = std::monostate{};