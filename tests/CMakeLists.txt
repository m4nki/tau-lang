cmake_minimum_required(VERSION 3.22.1 FATAL_ERROR)

set(DOCTEST_HEADER "${PROJECT_SOURCE_DIR}/src/doctest.h" CACHE PATH "Doctest header")
if (NOT EXISTS "${DOCTEST_HEADER}")
	message(STATUS "Downloading doctest to '${PROJECT_SOURCE_DIR}'")
	find_package(Wget REQUIRED)
	# TODO we should use a fixed tag instead of master
	execute_process(COMMAND "${WGET_EXECUTABLE}" https://raw.githubusercontent.com/doctest/doctest/master/doctest/doctest.h -P ${PROJECT_SOURCE_DIR}/src)
endif ()

add_library(doctest INTERFACE)
target_compile_definitions(doctest INTERFACE TAU_USE_DOCTEST)
set(DOCTEST_CONFIG_ASSERTION_PARAMETERS_BY_VALUE "true")

add_link_options("-flto")

set(TEST_SAMPLE test_sample.cpp)
add_executable(test_sample ${TEST_SAMPLE})
target_setup(test_sample)
target_link_libraries(test_sample TAUo doctest)

set(TEST_BA test_ba.cpp)
add_executable(test_ba ${TEST_BA})
target_setup(test_ba)
target_link_libraries(test_ba TAUo doctest)

add_test(NAME test_sample COMMAND "${PROJECT_BINARY_DIR}/test_sample")
add_test(NAME test_ba COMMAND "${PROJECT_BINARY_DIR}/test_ba")