cmake_minimum_required(VERSION 3.5)

project(AstroXplorer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(app_icon_resource_windows "${CMAKE_CURRENT_SOURCE_DIR}/resource.rc")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Xml)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Multimedia)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        cfgloader.h
        cfgloader.cpp
        apihandler.h
        apihandler.cpp
        imagemanipulation.h
        imagemanipulation.cpp
        enums.h
        enums.cpp
        podcasts.h
        podcasts.cpp
        podcast.h
        podcast.cpp
        podcastepisode.h
        podcastepisode.cpp
        epic.h
        epic.cpp
        epicimage.h
        epicimage.cpp
        Position3D.h
        Coordinates2D.h
        maps.h
        maps.cpp
        maptile.h
        maptile.cpp
        maplayer.h
        maplayer.cpp
        tilematrixset.h
        tilematrixset.cpp
        TileMatrix.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(AstroXplorer
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${app_icon_resource_windows}
    )

# Define target properties for Android with Qt 6 as:
#    set_property(TARGET AstroXplorer APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(AstroXplorer SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(AstroXplorer
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(AstroXplorer PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(AstroXplorer PRIVATE Qt${QT_VERSION_MAJOR}::Network)
target_link_libraries(AstroXplorer PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(AstroXplorer PRIVATE Qt${QT_VERSION_MAJOR}::Gui)
target_link_libraries(AstroXplorer PRIVATE Qt${QT_VERSION_MAJOR}::Xml)
target_link_libraries(AstroXplorer PRIVATE Qt${QT_VERSION_MAJOR}::Multimedia)

set_target_properties(AstroXplorer PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS AstroXplorer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(AstroXplorer)
endif()
