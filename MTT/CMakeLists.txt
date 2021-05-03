cmake_minimum_required (VERSION 3.8)

add_executable(MTT)
set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT MTT)

set(MTT_HEADERS
"h/mainwindow.h"
"h/db_manager.h"
"h/file_manager.h"
"h/mapwidget.h"
"h/table_editor_widget.h"
"h/db_entry.h"
"h/entry_position.h"
"h/db_modal_dialog.h"
"h/mtt_utility.h"
"h/mtt_Coordinate.h"
"h/constants.h"
"h/circle_graphics_item.h"
"h/mtt_chart_manager.h"
"h/mtt_piechart.h"
"h/mtt_basechart.h"
"h/mtt_barchart.h"
)

set(MTT_SOURCES
"src/mtt_barchart.cpp"
"src/mtt_basechart.cpp"
"src/mtt_piechart.cpp"
"src/mtt_chart_manager.cpp"
"src/circle_graphics_item.cpp"
"src/mainwindow.cpp"
"src/db_manager.cpp"
"src/file_manager.cpp"
"src/mapwidget.cpp"
"src/table_editor_widget.cpp"
"src/db_modal_dialog.cpp"
"src/MTT.cpp"
"src/mtt_utility.cpp"
)

qt5_add_resources(QT_RESOURCES
"resources/icons.qrc"
"resources/pictures.qrc"
)

target_sources(MTT PRIVATE ${MTT_HEADERS} ${MTT_SOURCES} ${QT_RESOURCES})
target_include_directories(MTT PRIVATE  ${Qt5Widgets_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/h)
target_link_libraries(MTT PRIVATE Qt5::Core Qt5::Widgets Qt5::Charts Qt5::Sql)
