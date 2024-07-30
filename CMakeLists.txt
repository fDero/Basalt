cmake_minimum_required(VERSION 3.22)

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

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  ${MAIN_BINARY}
  ${MAIN_FILE}
  ${SOURCE_FILES}
)

add_executable(
  ${TEST_BINARY}
  ${SOURCE_FILES}
  ${TEST_FILES}
)

target_compile_options(
  ${MAIN_BINARY}
  PRIVATE
  -O3
  -Wall
  -Wpedantic
  -Wfloat-equal
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
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(${TEST_BINARY})