include_directories(${Boost_INCLUDE_DIRS})

add_library(highwaycore
    facade.hpp
    facade.cpp
    kafka.hpp 
    kafka.cpp 
    state.hpp
    state.cpp
)

set_target_properties(highwaycore PROPERTIES CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions")

target_link_libraries(highwaycore PRIVATE RdKafka::rdkafka++ spdlog::spdlog)
target_include_directories(highwaycore INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
