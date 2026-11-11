#include "value.h"
#include <cmath>
#include <optional>
#include <stdexcept>
#include <string>
#include <variant>

std::string TypeToString(Type type) {
    switch (type) {
        case Type::Number:
            return "number";
        case Type::String:
            return "string";
        case Type::Boolean:
            return "boolean";
        case Type::Void:
            return "void";
        default:
            return "<error>";
    }
}

std::string Value::ToString() const {
    switch (type) {
        case Type::Number: {
            double num = *GetNum();
            return std::isnan(num) ? "NaN" : std::to_string(num);
        }

        case Type::String: {
            return *GetStr();
        }

        case Type::Boolean: {
            return *GetBool() ? "true" : "false";
        }

        default: {
            throw std::runtime_error("Unprintable type id=" + std::to_string(static_cast<int>(type)));
        }
    }
}

bool Value::IsTruthy() const {
    switch (type) {
        case Type::Number: {
            double num = *GetNum();

            return num != 0.0 && !std::isnan(num);
        }

        case Type::String: {
            return !GetStr()->empty();
        }

        case Type::Boolean: {
            return *GetBool();
        }

        case Type::Void: {
            return false; // void is always false
        }

        default: {
            throw std::runtime_error("Unknown truthiness type id=" + std::to_string(static_cast<int>(type)));
        }
    }
}

std::optional<double> Value::GetNum() const {
    if (type != Type::Number)
        return std::nullopt;

    const double *pnum = std::get_if<double>(&data);

    if (!pnum) {
        throw std::runtime_error("Value type does not match variant value. The "
                                 "type is number, but the variant isnt");
    }

    return *pnum;
}

std::optional<std::string> Value::GetStr() const {
    if (type != Type::String)
        return std::nullopt;

    const std::string *pstr = std::get_if<std::string>(&data);

    if (!pstr) {
        throw std::runtime_error("Value type does not match variant value. The "
                                 "type is string, but the variant isnt");
    }

    return *pstr;
}

std::optional<bool> Value::GetBool() const {
    if (type != Type::Boolean)
        return std::nullopt;

    const bool *pbool = std::get_if<bool>(&data);

    if (!pbool) {
        throw std::runtime_error("Value type does not match variant value. The "
                                 "type is boolean, but the variant isnt");
    }

    return *pbool;
}