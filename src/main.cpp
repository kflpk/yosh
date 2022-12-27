#include <iostream>
#include <string>
#include "prompt.hpp"
#include "shell.hpp"

int main() {
	Shell shell;

	shell.init();
	shell.loop();
}
