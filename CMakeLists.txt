cmake_minimum_required(VERSION 3.3)

project(QtFinder)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


cmake_policy(SET CMP0071 NEW)

find_package(Qt5 REQUIRED Core Widgets Gui)
qt5_wrap_ui(
  ui_files
  "${CMAKE_SOURCE_DIR}/include/QtFinderWindow.ui"
  )



set(src-files
  ${ui_files}
  "${CMAKE_SOURCE_DIR}/include/Events.h"
  "${CMAKE_SOURCE_DIR}/src/Events.cpp"

  "${CMAKE_SOURCE_DIR}/include/SearchLineEdit.h"
  "${CMAKE_SOURCE_DIR}/src/SearchLineEdit.cpp"

  "${CMAKE_SOURCE_DIR}/include/QuickfixWidget.h"
  "${CMAKE_SOURCE_DIR}/src/QuickfixWidget.cpp"


  "${CMAKE_SOURCE_DIR}/include/QtFinderWindow.h"
  "${CMAKE_SOURCE_DIR}/src/QtFinderWindow.cpp"

  "${CMAKE_SOURCE_DIR}/include/QtFinderAbstractComponents.h"

  "${CMAKE_SOURCE_DIR}/include/QtFinderApp.h"
  "${CMAKE_SOURCE_DIR}/src/QtFinderApp.cpp"

  "${CMAKE_SOURCE_DIR}/src/QtFinderApp_p.h"
  "${CMAKE_SOURCE_DIR}/src/QtFinderApp_p.cpp"

  "${CMAKE_SOURCE_DIR}/include/QtFinderTool.h"
  "${CMAKE_SOURCE_DIR}/src/QtFinderTool.cpp"

  "${CMAKE_SOURCE_DIR}/src/QtFinderToolImpl.h"
  "${CMAKE_SOURCE_DIR}/src/QtFinderToolImpl.cpp"

  "${CMAKE_SOURCE_DIR}/src/QtFinderWindowImpl.h"
  "${CMAKE_SOURCE_DIR}/src/QtFinderWindowImpl.cpp"

  "${CMAKE_SOURCE_DIR}/src/DesktopServiceImpl.h"
  "${CMAKE_SOURCE_DIR}/src/DesktopServiceImpl.cpp"

  "${CMAKE_SOURCE_DIR}/src/FileSystemScannerImpl.h"
  "${CMAKE_SOURCE_DIR}/src/FileSystemScannerImpl.cpp"

  "${CMAKE_SOURCE_DIR}/src/QtFinderAppCreater.cpp"
  "${CMAKE_SOURCE_DIR}/src/main.cpp"

  )

if(CMAKE_COMPILER_IS_GNUCC)
  set(linked_libries stdc++fs)
endif()

if(WIN32)
  set(GUI_TYPE WIN32)
elseif(APPLE)
  set(GUI_TYPE APPLE)
endif()

include_directories("./include")

add_executable(qt_finder ${GUI_TYPE} ${src-files})
target_include_directories(qt_finder PRIVATE "./include" ${CMAKE_BINARY_DIR})
target_link_libraries(qt_finder Qt5::Core Qt5::Widgets Qt5::Gui ${linked_libries})
enable_testing()
add_subdirectory(test)






