cmake_minimum_required(VERSION 3.5)

project(USB_GUI LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check https://doc.qt.io/qt/deployment-android.html for more information.
# They need to be set before the find_package( ...) calls below.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(USB_GUI
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(USB_GUI SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(USB_GUI
            ${PROJECT_SOURCES}
        )
    endif()
endif()

if (WIN32)
   include_directories("D:/msys2/mingw64/include/libusb-1.0")
   target_link_libraries(USB_GUI PRIVATE Qt${QT_VERSION_MAJOR}::Widgets "D:/msys2/mingw64/lib/libusb-1.0.a")
   add_definitions(-D WinX)
endif(WIN32)

if (APPLE)
    set(CMAKE_EXE_LINKER_FLAGS "-lobjc -framework IOKit -framework CoreFoundation")
    include_directories("/usr/local/Cellar/libusb/1.0.24/include/libusb-1.0")
    target_link_libraries(USB_GUI PRIVATE Qt${QT_VERSION_MAJOR}::Widgets "/usr/local/Cellar/libusb/1.0.24/lib/libusb-1.0.a")
    add_definitions(-D MacX)
endif(APPLE)
