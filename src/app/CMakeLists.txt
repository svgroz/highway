qt_add_executable(highway
    main_window.ui
    connection_properties.ui
    main.cpp
    ui.hpp
    ui.cpp
)

target_link_libraries(highway PRIVATE highwaycore Qt6::Core Qt6::Gui Qt6::Widgets RdKafka::rdkafka RdKafka::rdkafka++ lz4::lz4 spdlog::spdlog)

set_target_properties(highway PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)