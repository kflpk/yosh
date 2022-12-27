#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
#include <map>
#include "prompt.hpp"
#include "parser.hpp"

class Shell {
public:
    void welcome();
    unsigned int execute_command(std::string command);
    unsigned int execute(std::vector<std::string> args);
    int loop();
    void init();
private:
    Prompt prompt;
    Parser parser;

    // BUILTINS
    void exit();
    void cd(std::string path);
    void help();
    void export_var(std::string name, std::string value); 
    void assign_var(std::string name, std::string value);
    void display_vars(void);

    std::vector<std::string> history;
    std::map<std::string, std::string> variables;
    std::string getvar(std::string var);
};