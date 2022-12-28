#include "parser.hpp"
#include <iostream>
#include <algorithm>

std::vector<std::string> Parser::parse(std::string command) {
    std::vector<std::string> tokens;

    std::regex assign_rule(R"(\w+=\w+)");
    std::regex export_rule(R"(export \w+=\w+)");

    if(std::regex_match(command, assign_rule)) { // Match for assignment
        std::regex assign_group(R"((\w+)=(\w+))");
        std::smatch matches;
        std::regex_search(command, matches, assign_group);
        tokens.push_back("assign");
        tokens.push_back(matches[1]);
        tokens.push_back(matches[2]);
    } else if(std::regex_match(command, export_rule)) { // Match for export
        std::regex assign_group(R"(export (\w+)=(\w+))");
        std::smatch matches;
        std::regex_search(command, matches, assign_group);
        tokens.push_back("export");
        tokens.push_back(matches[1]);
        tokens.push_back(matches[2]);
        //TODO: Add double quotes support for assign and export
    } else {
        std::string buffer;
        std::regex token_regex(R"(\w+|\".+\")");
        std::sregex_iterator iter(command.begin(), command.end(), token_regex);
        std::sregex_iterator end;

        for(; iter != end; iter++) {
            buffer = iter->str(0);
            if(buffer.starts_with("\"") && buffer.ends_with("\"")) {
                // Strip buffer of double quotes
                buffer.erase(std::remove(buffer.begin(), buffer.end(), '\"'), buffer.end());
            }
            tokens.push_back(buffer);
        }
    }
    return tokens;
}