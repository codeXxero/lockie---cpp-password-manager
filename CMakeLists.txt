
cmake_minimum_required(VERSION 3.16)
 
project(lockie
    VERSION 1.0
    DESCRIPTION "Command Line Password Manager"
    LANGUAGES CXX
)
 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
 
include(GNUInstallDirs)
 
find_package(PkgConfig REQUIRED)
pkg_check_modules(SODIUM REQUIRED libsodium)
 
add_executable(lockie
    src/main.cpp
    src/passfunctions.cpp
)
 
target_include_directories(lockie PRIVATE
    src
    ${SODIUM_INCLUDE_DIRS}
)
 
target_link_libraries(lockie PRIVATE
    ${SODIUM_LIBRARIES}
)
 
target_compile_options(lockie PRIVATE ${SODIUM_CFLAGS_OTHER})
 
install(TARGETS lockie
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
 