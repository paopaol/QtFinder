find_package(Qt5 REQUIRED Test)

enable_testing()

qt5_wrap_ui(
  ui_files
  "${CMAKE_SOURCE_DIR}/include/private/QtFinderWindow.ui"
  )

set(will_test_files
  ${ui_files}
  "${CMAKE_SOURCE_DIR}/include/private/QtFinderWindow_p.h"
  "../src/SearchLineEdit.cpp"
  "../include/SearchLineEdit.h"
  "../src/QuickfixWidget.cpp"
  "../include/QuickfixWidget.h"
  "../src/Events.cpp"
  "../src/ui/QtFinderWindow_p.cpp"
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


# add_executable(QtFinderWindowPrivateTest ${will_test_files} QtFinderWindowPrivateTest.cpp)
# target_include_directories(QtFinderWindowPrivateTest PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
# target_link_libraries(QtFinderWindowPrivateTest Qt5::Core Qt5::Widgets Qt5::Gui Qt5::Test)
# add_test(QtFinderWindowPrivateTest QtFinderWindowPrivateTest)
