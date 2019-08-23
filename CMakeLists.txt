cmake_minimum_required (VERSION 2.8.8)
project (CSASM)

find_package(CMocka REQUIRED)

include_directories(
        ${PROJECT_SOURCE_DIR}/include
)

file(GLOB CSASM_SRC
    "${PROJECT_SOURCE_DIR}/include/csasm/*.h"
    "${PROJECT_SOURCE_DIR}/src/csasm/*.c"
    "${PROJECT_SOURCE_DIR}/include/*.h"
    "${PROJECT_SOURCE_DIR}/src/*.c"
)
add_executable(CSASM ${CSASM_SRC})

file(GLOB CSASM_SRC_TESTS
    "${PROJECT_SOURCE_DIR}/include/csasm/*.h"
    "${PROJECT_SOURCE_DIR}/include/cmocka/*.h"
    "${PROJECT_SOURCE_DIR}/src/csasm/*.c"
    "${PROJECT_SOURCE_DIR}/test/*.h"
    "${PROJECT_SOURCE_DIR}/test/*.c"
)
add_executable(TEST_CSASM ${CSASM_SRC_TESTS})
add_dependencies(CSASM TEST_CSASM)
target_link_libraries(TEST_CSASM ${PROJECT_SOURCE_DIR}/lib/libcmocka.so.0)

enable_testing()
add_test(TEST_CSASM CSASM)
add_custom_command(
    TARGET TEST_CSASM
    COMMENT "Run CSASM Unit Tests"
    POST_BUILD 
    COMMAND TEST_CSASM
)