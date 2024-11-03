cmake_minimum_required(VERSION 3.22)

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
#                                      PROJECT FILE-STRUCTURE SETTINGS                                      #
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#

project(
  Basalt
  DESCRIPTION "The Basalt Programming Language"
  HOMEPAGE_URL https://github.com/fDero/Basalt
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(TEST_BINARY basalt_unit_tests)
set(MAIN_BINARY basalt)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/lib)

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB_RECURSE MAIN_FILE ${CMAKE_SOURCE_DIR}/src/main.cpp)
file(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/*/*.cpp)
file(GLOB_RECURSE TEST_FILES ${CMAKE_SOURCE_DIR}/tests/*/*.cpp)

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
#                                             DEPENDENCIES                                                 #
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#

set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/dependencies/build/Release/generators ${CMAKE_PREFIX_PATH})
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/dependencies/build/Release/generators ${CMAKE_MODULE_PATH})

find_package(LLVM REQUIRED)
find_package(GTest REQUIRED)

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
#                                     "basalt" COMPILATION OPTIONS                                         #
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#

add_executable(
  ${MAIN_BINARY}
  ${MAIN_FILE}
  ${SOURCE_FILES}
)

target_compile_options(
  ${MAIN_BINARY}
  PRIVATE
  -O3
  -Wall
  -Wpedantic
  -Wfloat-equal
)

target_link_libraries(
  ${MAIN_BINARY}
  llvm-core::llvm-core
)

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
#                                 "basalt_unit_tests" COMPILATION OPTIONS                                  #
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#

add_executable(
  ${TEST_BINARY}
  ${SOURCE_FILES}
  ${TEST_FILES}
)

target_compile_options(
  ${TEST_BINARY}
  PRIVATE
  -O0
  -Wall
  -Wpedantic
  -Wfloat-equal
  -DDEBUG_BUILD
)

target_link_libraries(
  ${TEST_BINARY}
  PRIVATE
  GTest::GTest
  GTest::Main
  llvm-core::llvm-core
)
