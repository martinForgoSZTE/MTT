cmake_minimum_required (VERSION 3.8)

add_executable(MTT)
set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT MTT)

set(MTT_HEADERS
"h/MTT.h"
"h/mainwindow.h"
"h/db_manager.h"
"h/file_manager.h"
"h/mapwidget.h"
"h/table_editor_widget.h"
"h/db_entry.h"
"h/county.h"
"h/county_seat.h"
"h/region.h"
"h/large_region.h"
)

set(MTT_SOURCES
"src/mainwindow.cpp"
"src/db_manager.cpp"
"src/file_manager.cpp"
"src/mapwidget.cpp"
"src/table_editor_widget.cpp"
"src/MTT.cpp"
)

qt5_add_resources(QT_RESOURCES
"resources/icons.qrc"
"resources/pictures.qrc"
)

target_sources(MTT PRIVATE ${MTT_HEADERS} ${MTT_SOURCES} ${QT_RESOURCES})
target_include_directories(MTT PRIVATE  ${Qt5Widgets_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/h)
add_definitions(${Qt5Widgets_DEFINITIONS})
target_link_libraries(MTT PRIVATE Qt5::Core Qt5::Widgets Qt5::Charts Qt5::Sql)
