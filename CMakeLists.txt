cmake_minimum_required(VERSION 3.17)
project(ELEC2204)

set(CMAKE_CXX_STANDARD 14)

add_executable(ELEC2204 main.cpp makeMachine.cpp makeMachine.h memory.cpp memory.h include.h control.cpp control.h mipsProcessor.cpp mipsProcessor.h)