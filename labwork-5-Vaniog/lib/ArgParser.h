#pragma once
#include "Argument.h"

#include <string>
#include <set>
#include "map"
#include <vector>
#include <queue>

namespace ArgumentParser {

class ArgParser {
public:
    Argument<int32_t>& AddIntArgument(const std::string& name, const std::string& description = "");
    Argument<std::string>& AddStringArgument(const std::string& name, const std::string& description = "");
    Argument<float>& AddFloatArgument(const std::string& name, const std::string& description = "");
    Argument<bool>& AddFlag(const std::string& name, const std::string& description = "");

    Argument<int32_t>& AddIntArgument(char short_name, const std::string& name,
                                      const std::string& description = "");
    Argument<std::string>& AddStringArgument(char short_name, const std::string& name,
                                             const std::string& description = "");
    Argument<float>& AddFloatArgument(char short_name, const std::string& name,
                                             const std::string& description = "");
    Argument<bool>& AddFlag(char short_name, const std::string& name,
                            const std::string& description = "");

    [[nodiscard]] std::string GetStringValue(const std::string& name, uint32_t i = 0) const;
    [[nodiscard]] int32_t GetIntValue(const std::string& name, uint32_t i = 0) const;
    [[nodiscard]] float GetFloatValue(const std::string& name, uint32_t i = 0) const;
    [[nodiscard]] bool GetFlag(const std::string& name, uint32_t i = 0) const;

public:
    std::string parser_name;
    explicit ArgParser(std::string parser_name) : parser_name(std::move(parser_name)) {};
    ~ArgParser();

    bool Parse(int argc, char** argv);
    bool Parse(const std::vector<std::string>& argv);
    bool Parse(std::queue<std::string>& argv);

private:
    std::vector<std::pair<char, std::string>> arg_names;
    std::map<char, std::string> converter; // convert short names to long

    std::map<std::string, Argument<int32_t>*> int_args; // name and data
    std::map<std::string, Argument<std::string>*> string_args; // name and data
    std::map<std::string, Argument<float>*> float_args;
    std::map<std::string, Argument<bool>*> flag_args;
    std::string positional_arg_name{};

private:
    void FindPositional();
    void AddToPositional(const std::string& value);
    void ParseFlags(const std::string& flags_string); // like -oac
    bool CheckForParseErrors();

    void GetArgAndValue(std::string& arg_name,
                        std::string& value,
                        std::queue<std::string>& argv);
    // GetArgAndValue description:
    // input  | arg_name | value
    //--------------------------
    // --flag |  "flag"  | "1"
    // -oac   |  "oac"   | ""
    // --sm=3 |  "sm"    | "3"
    // --sm 3 |  "sm"    | "3"
    // -s 3   |  "s"     | "3"
    // 3      |  ""      | 3
    // -s=3   |  "s"     | "3"

    template<typename T>
    Argument<T>* NewArgument(char short_name, const std::string& name,
                             const std::string& description = "");

    std::string short_help_name{};
    std::string help_name{};

public:
    void AddHelp(char short_name, const std::string& name,
                 const std::string& description = "");
    [[nodiscard]] bool Help() const;
    std::string HelpDescription();
private:
    std::string DescriptionForArgument(const std::string& name);
};

} // namespace ArgumentParser