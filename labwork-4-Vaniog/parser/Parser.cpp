#include "Parser.h"

bool Parser::Parse(int argc, char** argv) {
    InitSettings();
    if (argc < 2) // non information
        return true;
    for (uint32_t i = 1; i < argc; i++) {
        std::string command = argv[i];
        std::string option = SplitStringByEqual(command).first;
        std::string value = SplitStringByEqual(command).second;
        ConvertOpt(option);
        if (IsParameter(option)) {
            if (value.empty()) {
                args.insert(std::make_pair(option, argv[i + 1]));
                i++;
            } else {
                args.insert(std::make_pair(option, value));
            }
        } else if (IsOption(option)) {
            args.insert(std::make_pair(option, "1"));
        } else {
            free_args.push_back(option);
        }
    }
    return InitValues();
}

bool Parser::IsParameter(const std::string& opt) {
    return are_parameters.find(opt) != are_parameters.end();
}
bool Parser::IsOption(const std::string& opt) {
    return are_options.find(opt) != are_options.end();
}

void Parser::ConvertOpt(std::string& opt) {
    if (option_convertor.find(opt) == option_convertor.end()) {
        return;
    }
    opt = option_convertor.find(opt)->second;
}

std::string Parser::GetParameter(const std::string& opt) {
    if (args.find(opt) == args.end()) {
        return "0";
    }
    return args[opt];
}

std::pair<std::string, std::string> Parser::SplitStringByEqual(const std::string& str) {
    std::string opt;
    std::string value;
    uint32_t i = 0;
    for (; i < str.size() && str[i] != '='; i++)
        opt.push_back(str[i]);
    i++;
    for (; i < str.size(); i++)
        value.push_back(str[i]);
    return std::make_pair(opt, value);
}
