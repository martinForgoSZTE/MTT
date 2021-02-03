cmake_minimum_required (VERSION 3.8)

add_executable(MTT)
set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT MTT)

set(MTT_HEADERS

)

set(MTT_SOURCES

)

target_sources(MTT PRIVATE ${MTT_HEADERS} ${MTT_SOURCES})
target_include_directories(MTT PRIVATE  ${Qt5Widgets_INCLUDES})
add_definitions(${Qt5Widgets_DEFINITIONS})
target_link_libraries(MTT PRIVATE Qt5::Core Qt5::Widgets Qt5::Charts)
