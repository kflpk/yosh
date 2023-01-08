#pragma once

#include <string>
#include <iostream>
#include <map>
#include <algorithm>

class Prompt {
public:
    void display();
    void set_prompt(std::string src);
    std::string get_prompt(void);
private:
    std::string prompt;
    std::map<std::string, std::string> colors = {
        {"%{default}", "\033[0m"},
        {"%{red}",     "\033[31m"},
        {"%{green}",   "\033[32m"},
        {"%{yellow}",  "\033[33m"},
        {"%{blue}",    "\033[34m"},
        {"%{magenta}", "\033[35m"},
        {"%{cyan}",    "\033[36m"},
        {"%{white}",   "\033[37m"},
    };
};