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
  "../src/ui/QtFinderWindow_p.cpp")

add_executable(gui_unit_test ${will_test_files} ui_main_delegete_test.cpp)
target_include_directories(gui_unit_test PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(gui_unit_test Qt5::Core Qt5::Widgets Qt5::Gui Qt5::Test)
add_test(gui_unit_test gui_unit_test)