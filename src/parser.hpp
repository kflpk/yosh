#pragma once

#include <string>
#include <algorithm>
#include <regex>
#include <map>
#include "unistd.h"

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
    // Parser(std::map<std::string, std::string>* vars_ref);
    Command parse(std::string command);
    char** parse_to_cstrings(std::vector<std::string> args);
    void pass_variables(std::map<std::string, std::string>* vars_ptr);
private:
    // Parser();
    std::vector<std::string> builtin_list;
    std::map<std::string, std::string>* shell_vars;
    std::string getvar(std::string var);
};