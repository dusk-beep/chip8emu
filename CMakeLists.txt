cmake_minimum_required(VERSION 3.10)

# Define the project name
project(Chip8Project)

# Set to export compile_commands.json
include_directories(/data/data/com.termux/files/usr/include/SDL2)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add your source files
add_executable(chip8 chip8.c)
