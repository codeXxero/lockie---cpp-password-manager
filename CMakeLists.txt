cmake_minimum_required(VERSION 3.16)

project(lockie
    VERSION 1.0
    DESCRIPTION "Command Line Password Manager"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)

add_executable(lockie
    src/main.cpp
    src/passfunctions.cpp
)

target_include_directories(lockie PRIVATE
    src
)

install(TARGETS lockie
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})