#include "Param.h"
#include "IstreamFuncs.h"

namespace omfl {

Param::Param(std::istream& is) {
    SkipSpaces(is);
    uint8_t peeked = is.peek();
    if (peeked == '"') {
        is_valid_ = ParseString(is);
    } else if (peeked == '-' || peeked == '+' || (peeked >= '1' && peeked <= '9')) {
        is_valid_ = ParseNumber(is);
    } else if (peeked == '[') {
        is_valid_ = ParseArray(is);
    } else {
        is_valid_ = ParseBool(is);
    }
}

bool Param::ParseString(std::istream& is) {
    is.ignore(); // to skip '"'
    std::string value = isf::GetWhile(is, [](uint8_t c){return c != '"';});
    if (!is) {
        return false;
    }
    is.ignore(); // to skip '"'
    value_ = value;
    return true;
}

bool Param::ParseNumber(std::istream& is) {
    std::string value;

    std::string signature;
    std::string before_dot;
    std::string after_dot;
    if (is.peek() == '+' || is.peek() == '-') {
        signature += static_cast<char>(is.get());
    }
    std::function<bool(uint8_t)> ch_isdigit = [](uint8_t c){return std::isdigit(c);};
    before_dot = isf::GetWhile(is, ch_isdigit);
    if (before_dot.empty()){
        return false;
    }

    if (is.peek() == '.'){
        is.ignore();
        after_dot = isf::GetWhile(is, ch_isdigit);
        if (after_dot.empty())
            return false;
    }

    if (after_dot.empty()){
        value_ = std::stoi(signature + before_dot);
    } else {
        value_ = std::stof(signature + before_dot + "." + after_dot);
    }
    return true;
}

bool Param::ParseArray(std::istream& is) {
    std::string value;
    is.ignore(); // to skip '['
    value_ = param_vector();
    while (true) {
        if (is.peek() == ']') {
            is.ignore();
            return true;
        }
        auto new_param = std::make_shared<Param>(is);
        if (!new_param->IsValid()) {
            return false;
        }
        std::get<param_vector>(value_).push_back(new_param);
        SkipSpaces(is);
        if (!is) {
            return false;
        }
        if (is.peek() == ',') {
            is.ignore();
            continue;
        }
        if (is.peek() == ']') {
            is.ignore();
            return true;
        }
        return false;
    }
}

bool Param::ParseBool(std::istream& is) {
    std::string value;
    is.width(4);
    is >> value;
    if (value == "true") {
        value_ = true;
        return true;
    }
    if (value == "fals" && is.peek() == 'e') {
        is.ignore();
        value_ = false;
        return true;
    }
    return false;
}
bool Param::IsValid() const {
    return is_valid_;
}

void Param::SkipSpaces(std::istream& is) {
    isf::IgnoreWhile(is, [](uint8_t c){return c == ' ';});
}

bool Param::IsInt() const {
    return std::holds_alternative<int>(value_);
}
bool Param::IsString() const {
    return std::holds_alternative<std::string>(value_);
}
bool Param::IsBool() const {
    return std::holds_alternative<bool>(value_);
}
bool Param::IsFloat() const {
    return std::holds_alternative<float>(value_);
}

bool Param::IsArray() const {
    return std::holds_alternative<param_vector>(value_);
}
int32_t Param::AsInt() const {
    return std::get<int>(value_);
}
std::string Param::AsString() const {
    return std::get<std::string>(value_);
}
bool Param::AsBool() const {
    return std::get<bool>(value_);
}
float Param::AsFloat() const {
    return std::get<float>(value_);
}

const Param& Param::operator[](uint32_t i) const {
    if (std::get<param_vector>(value_).size() <= i)
        return *std::make_shared<Param>();
    return *std::get<param_vector>(value_)[i];
}
int32_t Param::AsIntOrDefault(int32_t default_value) const {
    if (IsInt())
        return AsInt();
    return default_value;
}
std::string Param::AsStringOrDefault(const std::string& default_value) const {
    if (IsString())
        return AsString();
    return default_value;
}
bool Param::AsBoolOrDefault(bool default_value) const {
    if (IsBool())
        return AsBool();
    return default_value;
}
float Param::AsFloatOrDefault(float default_value) const {
    if (IsFloat())
        return AsFloat();
    return default_value;
}
Param::Param() = default;
Param::~Param() = default;

}