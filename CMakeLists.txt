cmake_minimum_required(VERSION 3.18)

project(contrAll LANGUAGES C CXX VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 99)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR}/generators)

find_package(libusb REQUIRED)
find_package(ftxui REQUIRED)

add_subdirectory(controller)
add_subdirectory(app)

