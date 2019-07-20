find_package(Qt5 REQUIRED Test)

enable_testing()

qt5_wrap_ui(
  ui_files
  "${CMAKE_SOURCE_DIR}/include/QtFinderWindow.ui"
  )

set(will_test_files
  ${ui_files}
  "../src/SearchLineEdit.cpp"
  "../include/SearchLineEdit.h"
  "../src/QuickfixWidget.cpp"
  "../include/QuickfixWidget.h"
  "../src/Events.cpp"
  "../src/QtFinderWindow.cpp"
  "../include/QtFinderWindow.h")

add_executable(gui_unit_test ${will_test_files} ui_main_delegete_test.cpp)
target_include_directories(gui_unit_test PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(gui_unit_test Qt5::Core Qt5::Widgets Qt5::Gui Qt5::Test)
add_test(gui_unit_test gui_unit_test)


# add_executable(gui_main_window_test ${will_test_files} gui_main_window_test.cpp)
# target_include_directories(gui_main_window_test PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
# target_link_libraries(gui_main_window_test Qt5::Core Qt5::Widgets Qt5::Gui Qt5::Test)
# add_test(gui_main_window_test gui_main_window_test)


add_executable(QtFinderWindowTest ${will_test_files} QtFinderWindowTest.cpp)
target_include_directories(QtFinderWindowTest PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(QtFinderWindowTest Qt5::Core Qt5::Widgets Qt5::Gui Qt5::Test)
add_test(QtFinderWindowTest QtFinderWindowTest)
