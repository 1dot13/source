include(GoogleTest)

add_executable(ja2_tests TranslationTables.cpp)
target_include_directories(ja2_tests PRIVATE "${CMAKE_SOURCE_DIR}/i18n")
target_link_libraries(ja2_tests PRIVATE gtest_main)
# not beside the game executables: CMAKE_RUNTIME_OUTPUT_DIRECTORY may be a game
# directory, and CI packages every build/*.exe
set_target_properties(ja2_tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
gtest_discover_tests(ja2_tests DISCOVERY_MODE PRE_TEST)
