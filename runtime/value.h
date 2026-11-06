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

std::string type_to_string(Type type);

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

    std::string to_string() const;

    std::optional<double> get_num() const;
    std::optional<std::string> get_str() const;
    std::optional<bool> get_bool() const;

    bool is_truthy() const;
};

inline const Value Error = std::monostate{};