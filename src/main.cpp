#include <iostream>
#include <string>
#include <regex>
#include "prompt.hpp"
#include "shell.hpp"
#include "utils.hpp"

int main() {
	Shell shell;

	shell.init();
	shell.loop();
}
