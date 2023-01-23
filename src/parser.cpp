#include "parser.hpp"
#include <iostream>
#include <algorithm>

Command Parser::parse(std::string command_line) {
    Command command;
    std::vector<std::string> tokens;

    // std::regex assign_rule(R"(\w+=\w+)");
    // std::regex export_rule(R"(export \w+=\w+)");
    std::regex assign_rule(R"(^\s*([A-Za-z_][A-Za-z0-9_]*)=("[^"]*"|[a-zA-Z0-9_\.\-/]+)\s*$)");
    std::regex export_rule(R"(^\s*export\s+([A-Za-z_][A-Za-z0-9_]*)=("[^"]*"|[a-zA-Z0-9_\.\-/]+)\s*$)");

    if(std::regex_match(command_line, assign_rule)) {
        std::smatch matches;
        std::regex assign_group(R"(([a-zA-Z_][A-Za-z0-9_]*)=(\S+))");
        std::regex assign_group_quoted(R"(^\s*([A-Za-z_][A-Za-z0-9_]*)=\x22([^"]*)\x22\s*)");

        command.type = CommandType::Assignment;
        
        if(std::regex_match(command_line, assign_group)) 
            std::regex_search(command_line, matches, assign_group);
        else if(std::regex_match(command_line, assign_group_quoted)) 
            std::regex_search(command_line, matches, assign_group_quoted);

        command.VariableName = matches[1];
        command.VariableValue = matches[2];
        // return command;
    } else if(std::regex_match(command_line, export_rule)) {
        std::smatch matches;
        std::regex export_group(R"(\s*export\s+([a-zA-Z_][[:alnum:]_]*)=(\S+))");
        std::regex export_group_quoted(R"(^\s*export\s+([A-Za-z_][A-Za-z0-9_]*)=\x22([^"]*)\x22\s*)");

        command.type = CommandType::Export;
        
        if(std::regex_match(command_line, export_group)) 
            std::regex_search(command_line, matches, export_group);
        else if(std::regex_match(command_line, export_group_quoted)) 
            std::regex_search(command_line, matches, export_group_quoted);

        command.VariableName  = matches[1];
        command.VariableValue = matches[2];
    } else {
        command.type = CommandType::Execution;

        std::string buffer;
        std::regex token_regex(R"([\w.\-/]+|\"[^"]*")");
        std::sregex_iterator iter(command_line.begin(), command_line.end(), token_regex);
        std::sregex_iterator end;

        for(; iter != end; iter++) {
            buffer = iter->str(0);
            if(buffer.starts_with("\"") && buffer.ends_with("\"")) {
                // Strip buffer of double quotes
                buffer.erase(std::remove(buffer.begin(), buffer.end(), '\"'), buffer.end());
            }
            tokens.push_back(buffer);
        }
        command.argv = tokens;
    }
    return command;
} 

char** Parser::parse_to_cstrings(std::vector<std::string> args) {
    int args_count = args.size();

    // FIX: EXTREMELY BAD IMPLEMENTATION, FIX THIS LATES PLS
    char** argv = (char**)malloc((args_count + 1) * sizeof(char*));
    argv[args_count] = NULL;

    for(int i = 0; i < args_count; i++) {
        argv[i] = strdup(args[i].c_str());
    }

    return argv;
}