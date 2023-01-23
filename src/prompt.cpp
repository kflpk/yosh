#include "prompt.hpp"
#include <unistd.h>

void Prompt::set_prompt(std::string src) {
    prompt = src;
}
void Prompt::display() {
    //TODO: Add special sequences for prompt such as user, hostname or time
    std::string output_str = prompt;
    // FIX: Move hostname variable to Shell class
    // FIX: Fix that fucking memory leak

    // TODO: previous exit code
    // TODO: colors

    try {
        char* hostname = (char*)malloc(1024); // FIX: MEMORY LEAK
        gethostname(hostname, 1024); // NOTE: This size doesn't change during runtime
        output_str.replace(output_str.find("%u"), 2, getenv("USER"));
        output_str.replace(output_str.find("%h"), 2, hostname);
        output_str.replace(output_str.find("%w"), 2, get_current_dir_name()); // NOTE: possible memory leak
    } catch(std::exception ex) {

    }

    for(auto kvp : colors) {
        size_t index;
        while( (index = output_str.find(kvp.first)) != std::string::npos) {
            output_str.replace(index, kvp.first.size(), kvp.second);
        }
    }
    
    std::cout << output_str << std::flush;
}