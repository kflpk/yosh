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
    parser.pass_variables(&variables);
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

    if(getvar("PROMPT").empty())
        assign_var("PROMPT", "%{yellow}[%u@%h %{cyan}%w%{yellow}]$: %{default}");
    
}

int Shell::loop() {
    while(true) {
        std::string command;
        
        prompt.set_prompt(getvar("PROMPT"));
        prompt.display();

        std::getline(std::cin,  command);
        history.push_back(command);
        execute_command(command);
    }
    // FIX: Bug when using Ctrl+D
}

unsigned int Shell::execute_command(std::string command_line) {
    if(command_line.empty())
        return 0;

    Command command;
    command = parser.parse(command_line);
    int return_value = execute(command);
    return return_value;
}

unsigned int Shell::execute(Command& command) {
    std::vector<std::string> args = command.argv;

    switch(command.type) {
        case CommandType::Builtin:
            handle_builtin(command);
            return 0;
        break;
        case CommandType::Assignment:
            assign_var(command.VariableName, command.VariableValue);
            return 0;
        break;
        case CommandType::Export:
            export_var(command.VariableName, command.VariableValue);
            return 0;
        break;
        case CommandType::Execution:
        default:
        break;
    }

    if(args.empty()) {
        return 0;
    }

    pid_t pid;
    if((pid = fork()) > 0) { // Parent on succesful fork
        int status;
        waitpid(pid, &status, 0);
    } else if(pid == -1) { // Parent on failed fork
        std::cout << std::strerror(errno) << std::endl;
    } else if(pid ==  0) { // Child
        int exec_return;
        char** argv = parser.parse_to_cstrings(args);
        // NOTE: argv is not freed after returning, because the child process dies anyway
        exec_return = execvp(argv[0], argv);
        std::cout << std::strerror(errno) << std::endl;
        exit();
    }
    //TODO: Return the programs exit value
    return 0;
}

void Shell::exit() {
    // FIX: use $XDG_CACHE_HOME/yosh/history
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
    else {
        char* c_var = getenv(var.c_str());

        // If not found, return empty string
        if(c_var == NULL)
            return std::string();
        else
            return c_var;
    }
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
    if(!dir.empty())
        chdir(dir.c_str());
    else
        chdir(getvar("HOME").c_str());
    // NOTE: Function works properly, but overall cd functionality doesn't work bc of the parser
}

void Shell::pwd() {
    std::cout << get_current_dir_name() << std::endl;
}

void Shell::handle_builtin(Command command) {
    std::vector<std::string> args = command.argv;
    try {
        if(args[0] == "help") {
            help();
        } else if(args[0] == "welcome") {
            welcome();
        } else if(args[0] == "vars") {
            display_vars();
        } else if(args[0] == "exit") {
            exit();
        } else if(args[0] == "cd") {
            cd(args[1]);
        }
    } catch(std::exception ex) {
        std::cout << "Error: invalid numer of arguments" << std::endl;
    }
}
