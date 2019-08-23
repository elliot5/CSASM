cmake_minimum_required (VERSION 2.8.8)
project (CSASM)

find_library(CMOCKA_LIBRARY libcmocka.so.0)
find_package(CMocka CONFIG REQUIRED)

include_directories(
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src
)

file(GLOB CSASM_SRC
    "${PROJECT_SOURCE_DIR}/include/csasm/*.h"
    "${PROJECT_SOURCE_DIR}/src/csasm/*.c"
    "${PROJECT_SOURCE_DIR}/include/*.h"
    "${PROJECT_SOURCE_DIR}/src/*.c"
)

file(GLOB CSASM_SRC_TESTS
    "${PROJECT_SOURCE_DIR}/include/csasm/*.h"
    "${PROJECT_SOURCE_DIR}/src/csasm/*.c"
    "${PROJECT_SOURCE_DIR}/test/*.h"
    "${PROJECT_SOURCE_DIR}/test/*.c"
)

add_executable(CSASM ${CSASM_SRC})
target_link_libraries(CSASM ${CMOCKA_LIBRARY})

add_executable(TEST_CSASM ${CSASM_SRC_TESTS})
target_link_libraries(TEST_CSASM ${CMOCKA_LIBRARY})
add_dependencies(CSASM TEST_CSASM)

enable_testing()
add_test(TEST_CSASM CSASM)
add_custom_command(
    TARGET TEST_CSASM
    COMMENT "Run CSASM Unit Tests"
    POST_BUILD 
    COMMAND TEST_CSASM
)