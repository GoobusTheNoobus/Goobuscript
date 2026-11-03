cmake_minimum_required(VERSION 3.20)

project(Goobuscript)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

add_executable(gs 
    main.cpp
    driver.cpp
    lexer.cpp 
    token.cpp
)