cmake_minimum_required(VERSION 3.8)
project(jpeg)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/lib)

file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_library(CodeLib ${SOURCES})
target_include_directories(CodeLib PUBLIC ${PROJECT_SOURCE_DIR}/src)

# ----- GoogleTest -----
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(jpeg_compression ${SOURCES})
target_link_libraries(jpeg_compression PRIVATE CodeLib)


file(GLOB TESTS ${PROJECT_SOURCE_DIR}/tests/*.cpp)
add_executable(jpeg_tests ${TESTS})

target_link_libraries(jpeg_tests
    PRIVATE
        gtest
        gtest_main
        CodeLib
)

include(GoogleTest)
gtest_discover_tests(jpeg_tests)