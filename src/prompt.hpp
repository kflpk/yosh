#pragma once

#include <string>
#include <iostream>

class Prompt {
public:
    void display();
    void set_prompt(std::string src);
    std::string get_prompt(void);
private:
    std::string prompt;
};