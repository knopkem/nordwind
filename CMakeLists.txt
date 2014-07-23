# Project Tufão
project(tufao)

cmake_minimum_required(VERSION 2.8)

set(project_policies
  CMP0001 # NEW: CMAKE_BACKWARDS_COMPATIBILITY should no longer be used.
  CMP0002 # NEW: Logical target names must be globally unique.
  CMP0003 # NEW: Libraries linked via full path no longer produce linker search paths.
  CMP0004 # NEW: Libraries linked may NOT have leading or trailing whitespace.
  CMP0005 # NEW: Preprocessor definition values are now escaped automatically.
  CMP0006 # NEW: Installing MACOSX_BUNDLE targets requires a BUNDLE DESTINATION.
  CMP0007 # NEW: List command no longer ignores empty elements.
  CMP0008 # NEW: Libraries linked by full-path must have a valid library file name.
  CMP0009 # NEW: FILE GLOB_RECURSE calls should not follow symlinks by default.
  CMP0010 # NEW: Bad variable reference syntax is an error.
  CMP0011 # NEW: Included scripts do automatic cmake_policy PUSH and POP.
  CMP0012 # NEW: if() recognizes numbers and boolean constants.
  CMP0013 # NEW: Duplicate binary directories are not allowed.
  CMP0014 # NEW: Input directories must have CMakeLists.txt
  CMP0020 # NEW: disable autolinking to qtmain as we have our own main() functions (new in Qt 5.1)
  CMP0043 # NEW: Ignore COMPILE_DEFINITIONS_<Config> properties
  )
  
foreach(policy ${project_policies})
  if(POLICY ${policy})
    cmake_policy(SET ${policy} NEW)
  endif()
endforeach()

# Metadata
set(TUFAO_VERSION_MAJOR 0)
set(TUFAO_VERSION_MINOR 8)
set(TUFAO_VERSION_PATCH 4)
set(TUFAO_VERSION ${TUFAO_VERSION_MAJOR}.${TUFAO_VERSION_MINOR}.${TUFAO_VERSION_PATCH})
set(TUFAO_BRIEF "An asynchronous web framework for C++ built on top of Qt")

# Dependencies

find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)

get_target_property(QMAKE_EXECUTABLE Qt5::qmake LOCATION)
function(QUERY_QMAKE VAR RESULT)
    exec_program("${QMAKE_EXECUTABLE}" ARGS "-query ${VAR}" RETURN_VALUE return_code OUTPUT_VARIABLE output)
    if(NOT return_code)
        file(TO_CMAKE_PATH "${output}" output)
        set(${RESULT} ${output} PARENT_SCOPE)
    endif(NOT return_code)
endfunction(QUERY_QMAKE)

query_qmake(QT_INSTALL_PLUGINS QT_PLUGINS_DIR)
query_qmake(QT_HOST_DATA QT_DATA_DIR)

set(QT_MKSPECS_DIR ${QT_DATA_DIR}/mkspecs)

# Configure options
option(GENERATE_DOC
    "Use Doxygen to generate the project documentation" OFF
)

option(ENABLE_TESTS
    "Generate and run tests" OFF
)


option(DISABLE_EXCEPTIONS
    "Whether Tufão should completely avoid using exceptions" OFF
)

set(BUFFER_SIZE 128 CACHE STRING
    "The default buffer size (in bytes) used by Tufão")
add_definitions(-DBUFFER_SIZE=${BUFFER_SIZE})

# Build info
set(TUFAO_LIBRARY "tufao")

# Install info
set(includedir include/tufao-${TUFAO_VERSION_MAJOR})
set(libdir "lib${LIB_SUFFIX}")

# Targets
add_subdirectory(src)
add_subdirectory(include)
add_subdirectory(pkg)

if (GENERATE_DOC)
    add_subdirectory(doc)
endif()

# uninstall target
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

# CPack installer
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${TUFAO_BRIEF}")
set(CPACK_PACKAGE_VERSION_MAJOR "${TUFAO_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${TUFAO_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${TUFAO_VERSION_PATCH}")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING.LESSER")
include(CPack)

# Testing
if(ENABLE_TESTS)
    include(CTest)
    mark_as_advanced(BUILD_TESTING)
endif()
