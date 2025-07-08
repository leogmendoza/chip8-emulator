cmake_minimum_required(VERSION 3.10)
project(chip8_emulator)

# Set C language standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Include dirs for headers
include_directories(
    ../core/inc
    ../platform/inc
)

# Automatically collect all .c files from core/src and platform/src
file(GLOB CORE_SOURCES ../core/src/*.c)
file(GLOB PLATFORM_SOURCES ../platform/src/platform_pc.c)

# Add PC-side entry point
set(MAIN_SOURCE main.c)

# Combine all sources
set(SOURCES
    ${CORE_SOURCES}
    ${PLATFORM_SOURCES}
    ${MAIN_SOURCE}
)

# Find SDL2 for graphics/input
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
add_definitions(-DSDL_MAIN_HANDLED)  # Tell SDL2 you're the captain now basically

# Define executable target
add_executable(chip8 ${SOURCES})
target_link_libraries(chip8 ${SDL2_LIBRARY})
