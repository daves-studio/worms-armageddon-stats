cmake_minimum_required(VERSION 3.5)
project(wormkitmod LANGUAGES CXX ASM-ATT)

add_library(main MODULE src/guicon.cpp src/hooks.s src/main.cpp src/json11.cpp)
set_target_properties(main PROPERTIES PREFIX wk)
