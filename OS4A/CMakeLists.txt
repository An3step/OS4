cmake_minimum_required(VERSION 3.10)

project(Receiver)

# Найти необходимые компоненты Boost
find_package(Boost REQUIRED COMPONENTS process interprocess)

# Создать исполняемый файл
add_executable(Receiver receiver.cpp)

# Связать библиотеки Boost с проектом
target_link_libraries(Receiver PRIVATE Boost::process Boost::interprocess)