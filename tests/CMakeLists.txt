# Get all file using glob

file(GLOB_RECURSE TEST_SRC_FILES CONFIGURE_DEPENDS "src/*.cpp")

foreach(TEST_SRC_FILE ${TEST_SRC_FILES})
    get_filename_component(TEST_FILE_NAME ${TEST_SRC_FILE} NAME)
    string(REPLACE ".cpp" "" TEST_FILE_NAME ${TEST_FILE_NAME})

    add_executable(${TEST_FILE_NAME} ${TEST_SRC_FILE})
    target_compile_features(${TEST_FILE_NAME} PUBLIC cxx_std_17)
    if (MSVC)
        target_compile_options(${TEST_FILE_NAME} PRIVATE /W3)
    else()
        target_compile_options(${TEST_FILE_NAME} PRIVATE -Wall -Wextra -Wpedantic -pedantic-errors)
    endif()

    target_link_libraries(${TEST_FILE_NAME} terminal_ctrl)
    add_test(NAME ${TEST_FILE_NAME} COMMAND ${TEST_FILE_NAME})
endforeach()