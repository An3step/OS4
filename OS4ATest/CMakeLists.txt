cmake_minimum_required(VERSION 3.10)

project(ReceiverTests)

# Используем vcpkg
set(CMAKE_TOOLCHAIN_FILE "C:/Apps/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

# Ищем только GTest (GMock теперь часть GTest)
find_package(GTest REQUIRED CONFIG)

# Добавление исполняемого файла для тестов
add_executable(ReceiverTests
    Receiver_func_test.cpp
    Receiver_func.cpp
)

# Подключение библиотек - теперь все через GTest::
target_link_libraries(ReceiverTests PRIVATE
    GTest::gtest
    GTest::gmock
    GTest::gtest_main
)

enable_testing()
add_test(NAME ReceiverTests COMMAND ReceiverTests)