include_directories(${Boost_INCLUDE_DIRS})

enable_testing()

add_executable(test_core 
    kafka_test.cpp
)
target_link_libraries(test_core PRIVATE highwaycore)


add_test(NAME test_core COMMAND test_core)
