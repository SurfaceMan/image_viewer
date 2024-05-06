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
        utils.h
        utils.cpp
        editor/circleeditor.h
        editor/circleeditor.cpp
        editor/ringeditor.h
        editor/ringeditor.cpp
        editor/rotatedrecteditor.h
        editor/rotatedrecteditor.cpp
        editor/recteditor.h
        editor/recteditor.cpp
        editor/polygoneditor.h
        editor/polygoneditor.cpp 
        editor/regioneditor.h
        editor/regioneditor.cpp
        label/circlelabel.h
        label/circlelabel.cpp
        label/ringlabel.h
        label/ringlabel.cpp
        label/rotatedrectlabel.h
        label/rotatedrectlabel.cpp
        label/rectlabel.h
        label/rectlabel.cpp
        label/polygonlabel.h
        label/polygonlabel.cpp 
        label/regionlabel.h
        label/regionlabel.cpp  
        label/imagelabel.h
        label/imagelabel.cpp 
)

add_executable(viewer
    ${PROJECT_SOURCES}
    res/resource.qrc
)

target_include_directories(viewer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(viewer PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_compile_options(viewer PRIVATE
        $<$<CXX_COMPILER_ID:Clang>:-fPIC -fvisibility=hidden -Wall -Wextra -Wpedantic -Wmisleading-indentation -Wunused -Wuninitialized -Wshadow -Wconversion -Werror>
)

#if(CMAKE_BUILD_TYPE)
#    string(TOLOWER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
#    message("Build Type:${BUILD_TYPE}")
#
#    if(${BUILD_TYPE} STREQUAL "debug")
#        target_compile_options(viewer PRIVATE
#            $<$<CXX_COMPILER_ID:MSVC>:>
#            $<$<CXX_COMPILER_ID:GNU>:-fsanitize=address,undefined>
#            $<$<CXX_COMPILER_ID:Clang>:-fsanitize=address,undefined>
#        )
#    endif()
#endif(CMAKE_BUILD_TYPE)

include(GNUInstallDirs)
install(TARGETS viewer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
