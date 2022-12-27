#include "parser.hpp"
#include <iostream>

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
    } else if(std::regex_match(command, export_rule)) { // Match for assignment
        std::regex assign_group(R"(export (\w+)=(\w+))");
        std::smatch matches;
        std::regex_search(command, matches, assign_group);
        tokens.push_back("export");
        tokens.push_back(matches[1]);
        tokens.push_back(matches[2]);
    } else {
        std::stringstream ss(command);
        std::string token;
        while(ss >> token) {
            tokens.push_back(token);
        }
    }
    return tokens;
}