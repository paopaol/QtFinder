cmake_minimum_required(VERSION 3.3)

project(QtFinder)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 REQUIRED Core Widgets Gui)


qt5_wrap_ui(
  ui_files
  "./ui/main.ui"
  )

set(src-files
  ${ui_files}
  "./src/main.cpp"
  "./include/SearchLineEdit.h"
  "./include/QuickfixWidget.h"
  "./src/QuickfixWidget.cpp"
  "./src/SearchLineEdit.cpp"
  "./src/QtFinderWindow.cpp"
  )

add_executable(qt_finder ${src-files})
target_include_directories(qt_finder PRIVATE "./include" ${CMAKE_BINARY_DIR})
target_link_libraries(qt_finder Qt5::Core Qt5::Widgets Qt5::Gui)




