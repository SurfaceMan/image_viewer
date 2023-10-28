cmake_minimum_required(VERSION 3.12)

project(viewer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        imageviewer.h
        imageviewer.cpp
        label.h
        label.cpp
        labeleditor.h
        labeleditor.cpp
        types.h
        labelcategory.h
        labelcategory.cpp
        labelrect.h
        labelrect.cpp
        labelpolygon.h
        labelpolygon.cpp
        labelfactory.h
        utils.h
        utils.cpp
)

add_executable(viewer
    ${PROJECT_SOURCES}
    res/resource.qrc
)

target_link_libraries(viewer PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

include(GNUInstallDirs)
install(TARGETS viewer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
