cmake_minimum_required(VERSION 3.10)

project(Receiver)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(Receiver Receiver.cpp Receiver_func.cpp)