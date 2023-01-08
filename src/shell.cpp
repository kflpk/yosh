#include "shell.hpp"
#include <fstream>
#include <cstdlib>
#include <cerrno>
#include <unistd.h>
#include <sys/wait.h>

void Shell::welcome() {
    std::cout << "Welcome to yosh (YOshua's SHell)\n" << 
                 "It's a basic Linux command line shell implementing basic functions \n" <<
                 "such as executing programs with specified arguments, manipulating environmental variables.\n" <<
                 "It does not support scripting.\n" <<
                 "type \"help\" for more information"
                 "Made by Kacper Filipek for a university project at AGH UST\n" << std::endl;
}

void Shell::help() {
    std::cout << "yosh by Kacper Filipek\n" <<
    "version 0.0.1\n\n" <<
    "Usage:\n" <<
    "In yosh every line is a separate command. By default a command is an execution of an external program with following arguments being passed to the executed program as argv\n" <<
    "For example, the following line:\n"
    "echo hello, world\n" <<
    "will result in executing the \"echo\" program, with \"hello,\" being passed as argv[1] and \"world\" being passed as argv[2].\n"
    "The shell also supports referencing and modyfying of environmental variables.\n" <<
    "To export an environmental variable, you use the \"export\" keyword, followed by the name of the variable, assignment operator (\"=\") and variable content. For example the following line:\n" <<
    "export VAR=TRUE\n" <<
    "will result in setting a new variable named \"VAR\", and assigning value of \"TRUE\" to it. Note that all environmental variables are stores and passed as a string of text\n" << 
    "To exit the shell use Ctrl+C or type \"exit\"\n" <<
    std::endl;

}

void Shell::init() {
    // Initialize shell
    std::filesystem::path config(std::string(std::getenv("XDG_CONFIG_HOME")));
    config /= "yosh/yoshrc"; // appends yosh/yoshrc to config directory
    
    std::ifstream rcfile;
    rcfile.open(config.string());

    if(rcfile.is_open()) {
        while(!rcfile.eof()) {
            std::string line;
            std::getline(rcfile, line);
            execute_command(line);
        }
    }

    std::filesystem::path cache(std::string(std::getenv("XDG_CACHE_HOME")));
    if(!std::filesystem::exists(cache/"yosh")) {
        std::filesystem::create_directory(cache/"yosh");
    }

    //TODO: Add setting prompt from variable
    prompt.set_prompt("[user@hostname]$: ");
}

int Shell::loop() {
    while(true) {
        std::string command;
        prompt.display();
        std::getline(std::cin,  command);
        this->history.push_back(command);
        execute_command(command);
    }
}

unsigned int Shell::execute_command(std::string command) {
    if(command.empty())
        return 0;

    std::vector<std::string> args = parser.parse(command);
    this->execute(args);
}

unsigned int Shell::execute(std::vector<std::string> args) {
    // Step 1: Check whether there are any characters in args vector
    if(args.empty())
        return 0;

    // Step 2: Evaluate builtins if applicable
    if(args[0] == "assign") {
        if(args.size() > 2)
            assign_var(args[1], args[2]);
        else {
            std::cout << "Bad usage of \"assign\"\n" <<
            "Usage:\n" <<
            "assign <name> <value>\n" << std::flush;
        }
    } else if(args[0] == "export") {
        export_var(args[1], args[2]);
    } else if(args[0] == "help") {
        help();
    } else if(args[0] == "welcome") {
        welcome();
    } else if(args[0] == "vars") {
        display_vars();
    } else if(args[0] == "exit") {
        exit();
    } else if(args[0] == "pwd") {
        pwd();
    } else if(args[0] == "cd") {
        cd(args[1]);
    }

    //Step 3: If command is not a builtin, start a new proccess
    pid_t pid;
    if( (pid = fork()) > 0) { // Parent on succesful fork
        int status;
        std::cout << "I'm a parent process" << std::endl;
        waitpid(pid, &status, 0);
        std::cout << "The child has been reaped!" << std::endl;
    } else if(pid == -1) { // Parent on failed fork
        std::cout << "Couldn't fork a child process\n" << std::endl;
        std::cout << std::strerror(errno) << std::endl;
    } else if(pid ==  0) { // Child
        int exec_return;
        std::cout << "I'm a child process" << std::endl;
        char** argv = parser.parse_to_cstrings(args);
        // NOTE: argv is not freed after returning, because the child process dies anyway
        exec_return = execvp(argv[0], argv);
        std::cout << std::strerror(errno) << std::endl;
        exit();
    }

    return 0;
}

void Shell::exit() {
    std::ofstream histfile("/home/kacper/.cache/yosh/history", std::ios::app);
    if(histfile.good()) {
        for(auto line : history) {
            histfile << line << "\n";
        }
    }
    histfile.close();

    std::exit(0);
}

std::string Shell::getvar(std::string var) {
    // Search local shell variables for var
    if(variables.count(var))
        return variables[var];
    // If not found, search in env variables
    else
        return getenv(var.c_str());
    // If not found, return empty string
}

void Shell::assign_var(std::string name, std::string value) {
    variables[name] = value;
}

void Shell::export_var(std::string name, std::string value) {
    setenv(name.c_str(), value.c_str(), true);
}

void Shell::display_vars() {
    for(auto var : variables) {
        std::cout << var.first << "=" << var.second << std::endl;
    }
}

void Shell::cd(std::string dir) {
    chdir(dir.c_str());
    // NOTE: Function works properly, but overall cd functionality doesn't work bc of the parser
}

void Shell::pwd() {
    std::cout << get_current_dir_name() << std::endl;
}