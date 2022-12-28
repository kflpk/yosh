#include "prompt.hpp"

void Prompt::set_prompt(std::string src) {
    prompt = src;
}
void Prompt::display() {
    //TODO: Add special sequences for prompt such as user, hostname or time
    std::cout << prompt << std::flush;
}