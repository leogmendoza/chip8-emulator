cmake_minimum_required(VERSION 3.10)
project(chip8_emulator)

# Set C language standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Automatically collect all .c files from core/src and platform/pc (and platform/src)
file(GLOB CORE_SOURCES core/src/*.c)
file(GLOB PLATFORM_SOURCES platform/pc/*.c)
file(GLOB PLATFORM_SRC_SOURCES platform/src/*.c)

# Add PC-side entry points
set(SOURCES
    ${CORE_SOURCES}
    ${PLATFORM_SOURCES}
    ${PLATFORM_SRC_SOURCES}
)   

add_executable(chip8 ${SOURCES})

# Include dirs for headers
target_include_directories(chip8 PRIVATE
    core/inc
    platform/inc
)

# Find SDL2 for graphics/input
find_package(SDL2 REQUIRED)
add_definitions(-DSDL_MAIN_HANDLED)  # Tell SDL2 you're the captain now basically

# Link SDL2 properly
target_link_libraries(chip8 PRIVATE SDL2::SDL2)
