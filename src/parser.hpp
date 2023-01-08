#pragma once

#include <string>
#include <algorithm>
#include <regex>

class Parser {
public:
    std::vector<std::string> parse(std::string command);
    char** parse_to_cstrings(std::vector<std::string> args);
};