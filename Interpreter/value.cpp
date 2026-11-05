#include "value.h"
#include <optional>
#include <stdexcept>
#include <string>
#include <variant>

std::string type_to_string(Type type) {
    switch (type) {
        case Type::Number: return "number";
        case Type::String: return "string";
        case Type::Boolean: return "boolean";
        case Type::Void: return "void";
        default: return "<error>";
    }
}

std::string Value::to_string() const {
    switch (type) {
        case Type::Number: {
            return std::to_string(*get_num());
        }

        case Type::String: {
            return *get_str();
        }

        case Type::Boolean: {
            return *get_bool() ? "true" : "false";
        }

        default: {
            throw std::runtime_error("Unprintable type id=" + std::to_string(static_cast<int>(type)));
        }
            
    }
}

std::optional<double> Value::get_num() const {
    if (type != Type::Number) 
        return std::nullopt;

    const double* pnum = std::get_if<double>(&data);

    if (!pnum) {
        throw std::runtime_error(
            "Value type does not match variant value. The type is number, but the variant isnt");
    }

    return *pnum;
}

std::optional<std::string> Value::get_str() const {
    if (type != Type::String) 
        return std::nullopt;

    const std::string* pstr = std::get_if<std::string>(&data);

    if (!pstr) {
        throw std::runtime_error(
            "Value type does not match variant value. The type is string, but the variant isnt");
    }

    return *pstr;
}

std::optional<bool> Value::get_bool() const {
    if (type != Type::Boolean) 
        return std::nullopt;

    const bool* pbool = std::get_if<bool>(&data);

    if (!pbool) {
        throw std::runtime_error(
            "Value type does not match variant value. The type is boolean, but the variant isnt");
    }

    return *pbool;
}