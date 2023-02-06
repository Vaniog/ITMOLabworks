#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include <variant>

namespace omfl {
class Param {
    typedef std::vector<std::shared_ptr<Param>> param_vector;
    std::variant<std::string,
                 int32_t,
                 float,
                 bool,
                 param_vector>
            value_;
public:
    Param();
    Param(std::istream& is);
    [[nodiscard]] bool IsValid() const;

    [[nodiscard]] bool IsInt() const;
    [[nodiscard]] bool IsString() const;
    [[nodiscard]] bool IsBool() const;
    [[nodiscard]] bool IsFloat() const;
    [[nodiscard]] bool IsArray() const;

    [[nodiscard]] int32_t AsInt() const;
    [[nodiscard]] std::string AsString() const;
    [[nodiscard]] bool AsBool() const;
    [[nodiscard]] float AsFloat() const;
    const Param& operator[](uint32_t i) const;

    [[nodiscard]] int32_t AsIntOrDefault(int32_t default_value) const;
    [[nodiscard]] std::string AsStringOrDefault(const std::string& default_value) const;
    [[nodiscard]] bool AsBoolOrDefault(bool default_value) const;
    [[nodiscard]] float AsFloatOrDefault(float default_value) const;

private:
    bool is_valid_ = true;
    static void SkipSpaces(std::istream& is);
    bool ParseString(std::istream& is);
    bool ParseNumber(std::istream& is);
    bool ParseBool(std::istream& is);
    bool ParseArray(std::istream& is);
public:
    virtual ~Param();
};

}