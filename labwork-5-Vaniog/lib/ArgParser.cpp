#include "ArgParser.h"

#include <sstream>

namespace ArgumentParser {

template<typename T>
Argument<T>* ArgParser::NewArgument(char short_name, const std::string& name, const std::string& description) {
    arg_names.emplace_back(short_name, name);
    converter.insert({short_name, name});
    return new Argument<T>(name, description);
}

ArgParser::~ArgParser() {
    for (const auto& arg : string_args) {
        delete arg.second;
    }
    for (const auto& arg : int_args) {
        delete arg.second;
    }
    for (const auto& arg : float_args) {
        delete arg.second;
    }
    for (const auto& arg : flag_args) {
        delete arg.second;
    }
}

Argument<int32_t>& ArgParser::AddIntArgument(const std::string& name, const std::string& description) {
    return AddIntArgument(' ', name, description);
}
Argument<std::string>& ArgParser::AddStringArgument(const std::string& name, const std::string& description) {
    return AddStringArgument(' ', name, description);
}
Argument<float>& ArgParser::AddFloatArgument(const std::string& name, const std::string& description) {
    return AddFloatArgument(' ', name, description);
}
Argument<bool>& ArgParser::AddFlag(const std::string& name, const std::string& description) {
    return AddFlag(' ', name, description);
}

Argument<int32_t>& ArgParser::AddIntArgument(char short_name,
                                             const std::string& name,
                                             const std::string& description) {
    auto* arg = NewArgument<int32_t>(short_name, name, description);
    int_args.insert({name, arg});
    return *arg;
}
Argument<std::string>& ArgParser::AddStringArgument(char short_name,
                                                    const std::string& name,
                                                    const std::string& description) {
    auto* arg = NewArgument<std::string>(short_name, name, description);
    string_args.insert({name, arg});
    return *arg;
}
Argument<float>& ArgParser::AddFloatArgument(char short_name,
                                             const std::string& name,
                                             const std::string& description) {
    auto* arg = NewArgument<float>(short_name, name, description);
    float_args.insert({name, arg});
    return *arg;
}
Argument<bool>& ArgParser::AddFlag(char short_name,
                                   const std::string& name,
                                   const std::string& description) {
    auto* arg = NewArgument<bool>(short_name, name, description);
    arg->PushValue("0");
    flag_args.insert({name, arg});
    return *arg;
}

int32_t ArgParser::GetIntValue(const std::string& name, uint32_t i) const {
    return int_args.find(name)->second->GetValue(i);
}
std::string ArgParser::GetStringValue(const std::string& name, uint32_t i) const {
    return string_args.find(name)->second->GetValue(i);
}
float ArgParser::GetFloatValue(const std::string& name, uint32_t i) const {
    return float_args.find(name)->second->GetValue(i);
}
bool ArgParser::GetFlag(const std::string& name, uint32_t i) const {
    return flag_args.find(name)->second->GetValue(i);
}

bool ArgParser::Parse(int argc, char** argv) {
    std::queue<std::string> argv_queue;
    for (int i = 0; i < argc; i++)
        argv_queue.push(argv[i]);
    return Parse(argv_queue);
}

bool ArgParser::Parse(const std::vector<std::string>& argv) {
    std::queue<std::string> argv_queue(std::deque<std::string>(argv.begin(), argv.end()));
    return Parse(argv_queue);
}

bool ArgParser::Parse(std::queue<std::string>& argv) {
    std::string arg_name;
    std::string value;
    FindPositional();
    argv.pop();
    while (!argv.empty()) {
        GetArgAndValue(arg_name, value, argv);
        if (arg_name.empty()) {
            AddToPositional(value);
            continue;
        }
        if (value.empty()) {
            ParseFlags(arg_name);
        }

        if (int_args.find(arg_name) != int_args.end()) {
            int_args.find(arg_name)->second->PushValue(value);
        } else if (string_args.find(arg_name) != string_args.end()) {
            string_args.find(arg_name)->second->PushValue(value);
        } else if (float_args.find(arg_name) != float_args.end()) {
            float_args.find(arg_name)->second->PushValue(value);
        } else if (flag_args.find(arg_name) != flag_args.end()) {
            flag_args.find(arg_name)->second->PushValue(value);
        }
    }

    if (flag_args.find(help_name) != flag_args.end()) {
        if (flag_args.find(help_name)->second->GetValue()) {
            return true;
        }
    }

    return CheckForParseErrors();
}

void ArgParser::FindPositional() {
    for (const auto& arg : int_args) {
        if (arg.second->is_positional)
            positional_arg_name = arg.second->name;
    }
    for (const auto& arg : string_args) {
        if (arg.second->is_positional)
            positional_arg_name = arg.second->name;
    }
    for (const auto& arg : float_args) {
        if (arg.second->is_positional)
            positional_arg_name = arg.second->name;
    }
}

void ArgParser::AddToPositional(const std::string& value) {
    if (int_args.find(positional_arg_name) != int_args.end()) {
        int_args.find(positional_arg_name)->second->PushValue(value);
    } else if (string_args.find(positional_arg_name) != string_args.end()) {
        string_args.find(positional_arg_name)->second->PushValue(value);
    } else if (float_args.find(positional_arg_name) != float_args.end()) {
        float_args.find(positional_arg_name)->second->PushValue(value);
    }
}

void ArgParser::ParseFlags(const std::string& flags_string) {
    for (char f : flags_string) {
        std::string flag = converter.find(f)->second;
        flag_args.find(flag)->second->PushValue("1");
    }
}

void ArgParser::GetArgAndValue(std::string& arg_name, std::string& value,
                               std::queue<std::string>& argv) { // description in ".h"
    arg_name = argv.front();
    argv.pop();
    if (arg_name[0] == '-' && arg_name[1] == '-') { // long name
        arg_name.erase(0, 2);
    } else if (arg_name[0] == '-') { // short name
        if (arg_name.find('=') == std::string::npos) { // flags
            value = "";
            arg_name.erase(arg_name.begin());
            return;
        }

        char key = arg_name[1];
        arg_name.erase(0, 2);
        arg_name = converter.find(key)->second + arg_name;
    } else { // positional
        value = arg_name;
        arg_name = "";
        return;
    }

    if (flag_args.find(arg_name) != flag_args.end()) {
        value = "1";
        return;
    }

    if (arg_name.find('=') != std::string::npos) {
        value = "";
        for (uint32_t i = arg_name.find('=') + 1; i < arg_name.size(); i++) {
            value += arg_name[i];
        }
        arg_name.erase(arg_name.find('='), std::string::npos);
    } else {
        value = argv.front();
        argv.pop();
    }
}

void ArgParser::AddHelp(char short_name, const std::string& name, const std::string& description) {
    short_help_name = short_name;
    help_name = name;
    auto* arg = NewArgument<bool>(short_name, name, description);
    arg->PushValue("0");
    flag_args.insert({name, arg});
}

bool ArgParser::Help() const {
    if (help_name.empty())
        return false;
    return flag_args.find(help_name)->second->GetValue();
}

std::string ArgParser::HelpDescription() {
    std::stringstream description;
    description << parser_name + "\n";
    description << flag_args.find(help_name)->second->short_description + "\n\n";
    for (const auto& name : arg_names) {
        if (name.second == help_name) {
            continue;
        }
        if (name.first != ' ') { // short name
            description << "-" << name.first << ",  ";
        } else {
            description << "     "; // 5 spaces
        }
        description << DescriptionForArgument(name.second) + "\n";
    }
    description << "\n";
    description << "-" + short_help_name + ",  ";
    description << "--" + help_name + " Display this help and exit\n";
    return description.str();
}

bool ArgParser::CheckForParseErrors() {
    for (const auto& arg : int_args) {
        if (!arg.second->IsDefined()) {
            return false;
        }
    }
    for (const auto& arg : string_args) {
        if (!arg.second->IsDefined()) {
            return false;
        }
    }
    for (const auto& arg : float_args) {
        if (!arg.second->IsDefined()) {
            return false;
        }
    }
    for (const auto& arg : flag_args) {
        if (!arg.second->IsDefined()) {
            return false;
        }
    }
    return true;
}

std::string ArgParser::DescriptionForArgument(const std::string& name) {
    for (const auto& arg : int_args) {
        if (arg.second->name == name) {
            return arg.second->Description();
        }
    }
    for (const auto& arg : string_args) {
        if (arg.second->name == name) {
            return arg.second->Description();
        }
    }
    for (const auto& arg : float_args) {
        if (arg.second->name == name) {
            return arg.second->Description();
        }
    }
    for (const auto& arg : flag_args) {
        if (arg.second->name == name) {
            return arg.second->Description();
        }
    }
    return "";
}
}