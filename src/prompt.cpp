#include "prompt.hpp"

void Prompt::set_prompt(std::string src) {
    prompt = src;
}
void Prompt::display() {
    std::cout << prompt << std::flush;
}