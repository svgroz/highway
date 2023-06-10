cmake_minimum_required(VERSION 3.16)

project(helloworld VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    # warning level 4
    add_compile_options(/W4)
else()
    # additional warnings
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(Boost 1.82.0 REQUIRED)

find_package(Qt6Core CONFIG REQUIRED)
find_package(Qt6Gui CONFIG REQUIRED)
find_package(Qt6Widgets CONFIG REQUIRED)
find_package(RdKafka CONFIG REQUIRED)
find_package(lz4 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

qt_standard_project_setup()

add_subdirectory(src/app)
add_subdirectory(src/core)
add_subdirectory(src/test)
