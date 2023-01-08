#pragma once
#include <string>
#include <vector>
#include <iostream>

std::ostream& operator<< (std::ostream& os, const std::vector<std::string>& vect);
std::ostream& operator<< (std::ostream& os, const char** vect);