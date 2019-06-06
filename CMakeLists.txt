cmake_minimum_required(VERSION 3.3)

project(QtFinder)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


cmake_policy(SET CMP0071 NEW)

find_package(Qt5 REQUIRED Core Widgets Gui)


qt5_wrap_ui(
  ui_files
  "./ui/main.ui"
  )

set(src-files
  ${ui_files}
  "./src/main.cpp"
  "./include/SearchLineEdit.h"
  "./src/SearchLineEdit.cpp"
  "./include/QuickfixWidget.h"
  "./src/QuickfixWidget.cpp"
  "./src/QtFinderWindow.cpp"
  "./include/QtFinderWindow.h"
  "./include/QThread11.h"
  )

if(CMAKE_COMPILER_IS_GNUCC)
  set(inked_libries stdc++fs)
endif()

add_executable(qt_finder ${src-files})
target_include_directories(qt_finder PRIVATE "./include" ${CMAKE_BINARY_DIR})
target_link_libraries(qt_finder Qt5::Core Qt5::Widgets Qt5::Gui ${linked_libries})




