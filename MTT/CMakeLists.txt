cmake_minimum_required (VERSION 3.8)

add_executable(MTT)
set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT MTT)

set(MTT_HEADERS
"h/MTT.h"
"h/mainwindow.h"
)

set(MTT_SOURCES
"src/mainwindow.cpp"
"src/MTT.cpp"
)

qt5_add_resources(QT_RESOURCES
"resources/icons.qrc"
"resources/pictures.qrc"
)

target_sources(MTT PRIVATE ${MTT_HEADERS} ${MTT_SOURCES} ${QT_RESOURCES})
target_include_directories(MTT PRIVATE  ${Qt5Widgets_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/h)
add_definitions(${Qt5Widgets_DEFINITIONS})
target_link_libraries(MTT PRIVATE Qt5::Core Qt5::Widgets Qt5::Charts)
