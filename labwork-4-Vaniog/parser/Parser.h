#pragma once
#include <string>
#include <vector>
#include <map>
#include <set>

class Parser {
protected:
    std::map<std::string, std::string> args;
    std::vector<std::string> free_args;

    std::map<std::string, std::string> option_convertor; // convert short names to normal (example: "-c" -> "--create")
    std::set<std::string> are_options; // which names are options have
    std::set<std::string> are_parameters; // which names are parameters have

    bool IsParameter(const std::string& opt);
    bool IsOption(const std::string& opt);

    void ConvertOpt(std::string& opt);
    static std::pair<std::string, std::string> SplitStringByEqual(const std::string& str); // by '='

    virtual void InitSettings() = 0;
    virtual bool InitValues() = 0;
public:
    bool Parse(int argc, char** argv); // return true if succeed
    std::string GetParameter(const std::string& opt);
};