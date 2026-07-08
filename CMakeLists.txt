cmake_minimum_required(VERSION 3.20)
project(newcx CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra -fsanitize=address,undefined")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "-fsanitize=address,undefined")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

add_library(newcx_core
    src/common/cli.cpp
)

target_include_directories(newcx_core PUBLIC include)

add_executable(newc src/newc/main.cpp)
target_link_libraries(newc PRIVATE newcx_core)

add_executable(newcpp src/newcpp/main.cpp)
target_link_libraries(newcpp PRIVATE newcx_core)
