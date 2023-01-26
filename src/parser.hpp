#pragma once

#include <string>
#include <algorithm>
#include <regex>

enum class CommandType {
    Builtin,
    Execution,
    Assignment,
    Export,
    _sentinel
};

struct Command {
    CommandType type;
    int argc;
    std::vector<std::string> argv;
    std::string VariableName;
    std::string VariableValue;
};

class Parser {
public:
    Parser();
    Command parse(std::string command);
    char** parse_to_cstrings(std::vector<std::string> args);

    std::vector<std::string> builtin_list;
};