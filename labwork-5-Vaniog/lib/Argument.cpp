#include "Argument.h"
#include <string>

namespace ArgumentParser {

template<typename T>
Argument<T>& Argument<T>::StoreValue(T& value_to_store) {
    delete value;
    value = &value_to_store;
    return *this;
}

template<typename T>
Argument<T>& Argument<T>::MultiValue(uint32_t min_size) {
    multi_params = new MultiParams(min_size);
    return *this;
}

template<typename T>
Argument<T>& Argument<T>::StoreValues(std::vector<T>& new_values) {
    delete multi_params->values;
    multi_params->values = &new_values;
    return *this;
}

template<typename T>
Argument<T>& Argument<T>::Positional() {
    is_positional = true;
    return *this;
}

template<typename T>
Argument<T>& Argument<T>::Default(const T& new_default_value) {
    default_value = new T(new_default_value);
    is_defined = true;
    value = new T(new_default_value);
    return *this;
}

template<typename T>
void Argument<T>::PushValue(const std::string& new_value) {
    is_defined = true;
    if (multi_params != nullptr) {
        multi_params->values->push_back(StringToValue(new_value));
    } else {
        *value = StringToValue(new_value);
    }
}

template<typename T>
T Argument<T>::GetValue(uint32_t i) const {
    if (multi_params == nullptr) {
        return *value;
    }
    return multi_params->values->at(i);
}

template<typename T>
Argument<T>::Argument(std::string name, std::string description)
        : name(std::move(name)), short_description(std::move(description)) {}

template<typename T>
Argument<T>::MultiParams::MultiParams(uint32_t min_size) : min_size(min_size) {
    values = new std::vector<T>();
}

template<typename T>
bool Argument<T>::IsDefined() {
    if (multi_params != nullptr) {
        return multi_params->values->size() >= multi_params->min_size;
    }
    return is_defined;
}

template<typename T>
std::string Argument<T>::Description() {
    std::string description = "--" + name + GetType() + ",  ";
    description += short_description;
    if (is_positional) {
        description += " [positional]";
    }
    if (default_value != nullptr) {
        description += " [default = " +
                ValueToString(*default_value) + "]";
    }
    if (multi_params != nullptr) {
        description += " [repeated, min args = " +
                std::to_string(multi_params->min_size) + "]";
    }
    return description;
}

template<typename T>
std::string Argument<T>::GetType() {
    if (typeid(T) == typeid(int32_t)) {
        return "=<int>";
    }
    if (typeid(T) == typeid(std::string)) {
        return "=<string>";
    }
    if (typeid(T) == typeid(float)) {
        return "=<float>";
    }
    if (typeid(T) == typeid(bool)) {
        return "";
    }
    return "";
}

template<>
int32_t Argument<int32_t>::StringToValue(const std::string& str) { // NOLINT
    return std::stoi(str);
}
template<>
std::string Argument<std::string>::StringToValue(const std::string& str) { // NOLINT
    return str;
}
template<>
float Argument<float>::StringToValue(const std::string& str) { // NOLINT
    return std::stof(str);
}
template<>
bool Argument<bool>::StringToValue(const std::string& str) { // NOLINT
    return std::stoi(str);
}

template<>
std::string Argument<int32_t>::ValueToString(const int32_t& x) {
    return std::to_string(x);
}
template<>
std::string Argument<std::string>::ValueToString(const std::string& x) {
    return x;
}
template<>
std::string Argument<float>::ValueToString(const float& x) {
    return std::to_string(x);
}
template<>
std::string Argument<bool>::ValueToString(const bool& x) {
    return (x) ? "true" : "false";
}

template
class Argument<int32_t>;
template
class Argument<std::string>;
template
class Argument<float>;
template
class Argument<bool>;
} // namespace ArgumentParser