project(highway)
cmake_minimum_required(VERSION 3.25)

set (CMAKE_CXX_STANDARD 20)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6Core CONFIG REQUIRED)
find_package(Qt6Gui CONFIG REQUIRED)
find_package(Qt6Widgets CONFIG REQUIRED)
find_package(RdKafka CONFIG REQUIRED)
find_package(lz4 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_executable(${PROJECT_NAME}
    src/untitled.ui
    src/main.cpp
    src/kafka.hpp src/kafka.cpp 
    src/state.hpp src/state.cpp
    src/ui.hpp src/ui.cpp
    )
target_link_libraries(${PROJECT_NAME} PUBLIC PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets RdKafka::rdkafka RdKafka::rdkafka++ lz4::lz4 spdlog::spdlog)
