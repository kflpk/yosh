#include "utils.hpp"

std::ostream& operator<<(std::ostream& os, const std::vector<std::string>& vect) {
    for(std::string str : vect) {
        os << str << "\n";
    }
    return os;
}

std::ostream& operator<< (std::ostream& os, const char** strarr) {
    for(int i = 0; strarr[i] != NULL; i++) {
        std::cout << strarr[i] << "\n";
    }
    return os;
}