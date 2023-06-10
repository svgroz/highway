include_directories(${Boost_INCLUDE_DIRS})

add_library(highwaycore
    facade.hpp
    facade.cpp
    kafka.hpp 
    kafka.cpp 
    state.hpp
    state.cpp
)

target_link_libraries(highwaycore PRIVATE RdKafka::rdkafka++ spdlog::spdlog)
target_include_directories(highwaycore INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
