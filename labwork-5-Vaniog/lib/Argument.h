#pragma once
#include <vector>
#include <iostream>

namespace ArgumentParser {
class ArgParser;
template<typename T>
class Argument {
    friend class ArgParser;
public:
    Argument& StoreValue(T& value_to_store);

    Argument& MultiValue(uint32_t min_size = 0);
    Argument& StoreValues(std::vector<T>& new_values);

    Argument& Positional();
    Argument& Default(const T& new_default_value);

private:
    //void PushValue(const std::string& str);
    void PushValue(const std::string& new_value);
    [[nodiscard]] T GetValue(uint32_t i = 0) const;

    bool IsDefined();
    bool is_positional = false;

    std::string name;
    explicit Argument(std::string name,
                      std::string description = "");
private:
    T* value = new T();

    T* default_value = nullptr;
    struct MultiParams;

    MultiParams* multi_params = nullptr;
    bool is_defined = false;

    struct MultiParams {
        uint32_t min_size;
        std::vector<T>* values;
        explicit MultiParams(uint32_t min_size = 0);
    };

private:
    std::string short_description;
    std::string Description();
private:
    std::string GetType();
    T StringToValue(const std::string& str);
    std::string ValueToString(const T& x);
};
}