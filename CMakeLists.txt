cmake_minimum_required(VERSION 3.18)

project(contrAll LANGUAGES C CXX VERSION 1.0.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR}/generators)

find_package(libusb REQUIRED)
find_package(ftxui REQUIRED)

add_subdirectory(app)

