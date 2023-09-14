include_directories(${Boost_INCLUDE_DIRS})

add_library(highwaycore
    consumer_fsm.hpp
    lib.cpp
    hw.cpp
)

target_link_libraries(highwaycore PRIVATE spdlog::spdlog JNI::JNI)
target_include_directories(highwaycore INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
