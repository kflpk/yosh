#include "prompt.hpp"
#include <unistd.h>

void Prompt::set_prompt(std::string src) {
    prompt = src;
}
void Prompt::display() {
    std::string output_str = prompt;

    // TODO: previous exit code

    try {
        gethostname(hostname, 256); 
        if(output_str.find("%u") != std::string::npos)
            output_str.replace(output_str.find("%u"), 2, getenv("USER"));
        if(output_str.find("%h") != std::string::npos)
            output_str.replace(output_str.find("%h"), 2, hostname);
        if(output_str.find("%w") != std::string::npos)
            output_str.replace(output_str.find("%w"), 2, get_current_dir_name());
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