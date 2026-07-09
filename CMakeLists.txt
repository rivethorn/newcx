cmake_minimum_required(VERSION 3.20)
project(newcx CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra -fsanitize=address,undefined")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "-fsanitize=address,undefined")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

add_library(newcx_core
    src/common/cli.cpp
    src/common/config.cpp
    src/common/files.cpp
    src/generators/c_generator.cpp
    src/generators/cxx_generator.cpp
)

target_include_directories(newcx_core PUBLIC include)

add_executable(newcx src/main.cpp)
target_link_libraries(newcx PRIVATE newcx_core)

install(TARGETS newcx DESTINATION bin)

install(CODE [[
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E create_symlink
            newcx
            $ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/newc
    )

    execute_process(
        COMMAND ${CMAKE_COMMAND} -E create_symlink
            newcx
            $ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/newcpp
    )
]])
