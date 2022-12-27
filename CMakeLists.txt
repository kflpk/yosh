cmake_minimum_required(VERSION 3.16.3)

project(yosh)

set(WARNINGS -Wall -Wextra -pedantic -Wshadow -Wno-pragma-once-outside-header -Wfloat-equal -Wundef -Wcast-align -Wwrite-strings -Wmissing-declarations -Wredundant-decls -Wshadow -Woverloaded-virtual -pedantic -Wswitch)
set(CMAKE_BUILD_TYPE Debug)

add_executable(${PROJECT_NAME} src/main.cpp src/prompt.cpp src/shell.cpp src/parser.cpp)

target_link_libraries(${PROJECT_NAME} ncurses)
target_compile_options(${PROJECT_NAME} PRIVATE ${WARNINGS})

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20)