cmake_minimum_required(VERSION 3.22.1 FATAL_ERROR)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(namespace "TAU")
set(PROJECT_NAME tau)
# TODO (LOW) check if tau is better than TAU
set(PROJECT_LIB_NAME TAU)
project("${PROJECT_NAME}"
	VERSION 0.0.1
	DESCRIPTION "IDNI's tau language"
)

option(TAU_BUILD_DOC               "Build documentation"                   OFF)
option(TAU_BUILD_STATIC_LIBRARY    "Build static library"                  OFF)
option(TAU_BUILD_SHARED_LIBRARY    "Build shared library"                  OFF)
option(TAU_BUILD_EXECUTABLE        "Build executable"                      OFF)
option(TAU_BUILD_SHARED_EXECUTABLE "Build shared executable"               OFF)
option(TAU_BUILD_TESTS             "Build tests"                           OFF)
option(TAU_BUILD_INTEGRATION       "Build integration tests"               OFF)
option(TAU_BUILD_REGRESSION        "Build regression tests"                OFF)
#option(TAU_BUILD_EXAMPLES          "Build examples"                        OFF)
# TODO (LOW) remove a singular TAU_GENERATE_PARSER in a near future
option(TAU_GENERATE_PARSER         "Generates parsers from TGF (obsolete)" OFF)
option(TAU_GENERATE_PARSERS        "Generates parsers from TGF"            OFF)
set_property(CACHE TAU_BUILD_DOC               PROPERTY STRINGS "OFF" "ON")
set_property(CACHE TAU_BUILD_STATIC_LIBRARY    PROPERTY STRINGS "OFF" "ON")
set_property(CACHE TAU_BUILD_SHARED_LIBRARY    PROPERTY STRINGS "OFF" "ON")
set_property(CACHE TAU_BUILD_EXECUTABLE        PROPERTY STRINGS "OFF" "ON")
set_property(CACHE TAU_BUILD_SHARED_EXECUTABLE PROPERTY STRINGS "OFF" "ON")
set_property(CACHE TAU_BUILD_TESTS             PROPERTY STRINGS "OFF" "ON")
set_property(CACHE TAU_BUILD_INTEGRATION       PROPERTY STRINGS "OFF" "ON")
set_property(CACHE TAU_BUILD_REGRESSION        PROPERTY STRINGS "OFF" "ON")
#set_property(CACHE TAU_BUILD_EXAMPLES          PROPERTY STRINGS "OFF" "ON")
set_property(CACHE TAU_GENERATE_PARSER         PROPERTY STRINGS "OFF" "ON")
set_property(CACHE TAU_GENERATE_PARSERS        PROPERTY STRINGS "OFF" "ON")

if(NOT TAU_BUILD_STATIC_LIBRARY AND NOT TAU_BUILD_SHARED_LIBRARY
	AND NOT TAU_BUILD_EXECUTABLE AND NOT TAU_BUILD_SHARED_EXECUTABLE
	AND NOT TAU_BUILD_TESTS) # AND NOT TAU_BUILD_EXAMPLES)
		set(TAU_BUILD_STATIC_LIBRARY ON)
		set(TAU_BUILD_EXECUTABLE ON)
		set(TAU_BUILD_TESTS ON)
		set(TAU_BUILD_INTEGRATION ON)
		# enable once we have regression tests
		#set(TAU_BUILD_REGRESSION ON)
		#set(TAU_BUILD_EXAMPLES true)
endif()

message(STATUS "TAU_BUILD_STATIC_LIBRARY: ${TAU_BUILD_STATIC_LIBRARY}")
message(STATUS "TAU_BUILD_SHARED_LIBRARY: ${TAU_BUILD_SHARED_LIBRARY}")
message(STATUS "TAU_BUILD_EXECUTABLE: ${TAU_BUILD_EXECUTABLE}")
message(STATUS "TAU_BUILD_SHARED_EXECUTABLE: ${TAU_BUILD_SHARED_EXECUTABLE}")
message(STATUS "TAU_BUILD_TESTS: ${TAU_BUILD_TESTS}")
message(STATUS "TAU_BUILD_REGRESSION: ${TAU_BUILD_REGRESSION}")
#message(STATUS "TAU_BUILD_EXAMPLES: ${TAU_BUILD_EXAMPLES}")
message(STATUS "TAU_GENERATE_PARSERS: ${TAU_GENERATE_PARSERS}")

#
# Adding Boost log library
#
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED COMPONENTS log)
if (Boost_FOUND)
	message(STATUS "Boost version: ${Boost_VERSION}")
	message(STATUS "Boost include dirs: ${Boost_INCLUDE_DIRS}")
	message(STATUS "Boost libraries: ${Boost_LIBRARIES}")
else()
	message(FATAL_ERROR "Boost not found")
endif()

#
# Adding parser library's git submodule
#
option(PARSER_BUILD_STATIC_LIBRARY "build only static library" ON)
include(add-git-submodule)
add_git_submodule("external/parser")
set(IDNI_PARSER_OBJECT_LIB "idni_parsero")

# TODO (LOW) make it run automatically on grammar modifications
#
# Check https://stackoverflow.com/questions/26193068/how-to-only-build-auto-generated-code-when-the-generator-or-input-changes-in-cma

#
# Generate C++ parser from TGF if requested
#
if (TAU_GENERATE_PARSERS OR TAU_GENERATE_PARSER)
	include(generate-parser)
	generate_parser("${PROJECT_SOURCE_DIR}/parser/tau.tgf" tau_parser)
	generate_parser("${PROJECT_SOURCE_DIR}/parser/bdd.tgf" bdd_parser)
endif()

# load common cmake settings and load functions
# 		target_setup(), target_compile_definitions_if() and exclude()
include(tau-common)
#
# Adding src dir
#
add_subdirectory("src")

#if (TAU_BUILD_EXAMPLES)
#	add_subdirectory("examples")
#endif(TAU_BUILD_EXAMPLES)

#
# Testing
#
if (TAU_BUILD_TESTS)
	enable_testing()
	add_subdirectory(tests)
endif ()

#
# Build documentation
#
# check if Doxygen is installed
find_package(Doxygen)
if (DOXYGEN_FOUND AND TAU_BUILD_DOC)
	# set input and output files
	set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
	set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
	# request to configure the file
	configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
	message("Doxygen build started")
	# note the option ALL which allows to build the docs together with the application
	add_custom_target(doc_doxygen ALL
		COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMENT "Generating API documentation with Doxygen"
		VERBATIM
	)
else (DOXYGEN_FOUND AND TAU_BUILD_DOC)
	message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND AND TAU_BUILD_DOC)
