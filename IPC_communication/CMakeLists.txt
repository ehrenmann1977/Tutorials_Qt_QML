cmake_minimum_required(VERSION 3.5)

project(node LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5Core)
find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

if (ANDROID)
    add_library(node SHARED
        main.cpp
        nmsgpack.cpp
        nmsgpack.h
    )
else()
  add_executable(node
  main.cpp
  nmsgpack.cpp
  nmsgpack.h
)
endif()


#target_link_libraries(node Qt5::Core)

# example under win32:
#if (WIN32)
#   include_directories("D:/msys2/mingw64/include/libusb-1.0")
#   target_link_libraries(USB_GUI PRIVATE Qt${QT_VERSION_MAJOR}::Widgets "D:/msys2/mingw64/lib/libusb-1.0.a")
#   add_definitions(-D WinX)
#endif(WIN32)

if (APPLE)
    #set(CMAKE_EXE_LINKER_FLAGS "-lobjc -framework IOKit -framework CoreFoundation")
    include_directories("./lib")
    include_directories("./libs/include/")
    include_directories("./libs/include/msgpack")
    include_directories("./libs/include/nanomsg")
    target_link_libraries(node PRIVATE Qt5::Core PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${PROJECT_SOURCE_DIR}/libs/lib/libmsgpackc.a ${PROJECT_SOURCE_DIR}//libs/lib/libnanomsg.a)
    add_definitions(-D MacX)
endif(APPLE)
