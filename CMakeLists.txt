cmake_minimum_required(VERSION 3.26)

project("terminal_ctrl" VERSION 0.1.0 LANGUAGES CXX)

option(TERMINALCTRL_BUILD_TESTING "Build the tests" ${PROJECT_IS_TOP_LEVEL})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE})

set(TERMINAL_CTRL_HEADER_FILES "src/terminal_ctrl.hpp")

add_library(${PROJECT_NAME} INTERFACE)
add_library("${PROJECT_NAME}::${PROJECT_NAME}" ALIAS ${PROJECT_NAME})

set(TERMINAL_CTRL_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}")

target_include_directories(
    ${PROJECT_NAME}
    INTERFACE
    $<BUILD_INTERFACE:${TERMINAL_CTRL_INCLUDES}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

if (TERMINALCTRL_BUILD_TESTING)
    add_subdirectory(tests)
endif()