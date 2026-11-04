#pragma once
#include <optional>
#include <string>
#include <variant>

enum class Type {
    Number,
    String,
    
    Void,
    Error,
};

std::string type_to_string(Type type);

struct Value {
    std::variant<double, std::string, std::monostate> data;
    Type type;

    Value(double data) : data(data), type(Type::Number) {}
    Value(std::string data) : data(std::move(data)), type(Type::String) {}
    Value() : type(Type::Void) {}
    Value(std::monostate) : type(Type::Error) {}

    operator bool() const { 
        return type != Type::Error; 
    }

    std::string to_string() const;

    std::optional<double> get_num() const;
    std::optional<std::string> get_str() const;
};

inline const Value Error = std::monostate{};