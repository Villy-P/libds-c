cmake_minimum_required(VERSION 3.10)
project(LIBDS_C VERSION 1.0)

set(EXEC_NAME_TEST "libds-c-test")
set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB_RECURSE LIBDS_C_TEST_SOURCES CONFIGURE_DEPENDS "test/*.c")
list(FILTER LIBDS_C_TEST_SOURCES EXCLUDE REGEX ".*src/main.c$")

include(FetchContent)
fetchcontent_declare(cpptrace
    GIT_REPOSITORY https://github.com/jeremy-rifkin/cpptrace.git
    GIT_TAG        v0.7.5
)
fetchcontent_makeavailable(cpptrace)

add_library(common_flags INTERFACE)

target_compile_options(common_flags INTERFACE
    -Wall 
    -Wextra 
    -g 
    -O0
    $<$<PLATFORM_ID:Windows>:-gcodeview>
    $<$<CONFIG:Debug>:-fdiagnostics-color=always>
)

target_link_options(common_flags INTERFACE
    -g
    $<$<PLATFORM_ID:Windows>:-Wl,--export-all-symbols>
    $<$<PLATFORM_ID:Windows>:-Wl,--enable-auto-image-base>
)

add_executable(${EXEC_NAME_TEST} ${LIBDS_C_TEST_SOURCES})
target_include_directories(${EXEC_NAME_TEST} PRIVATE include/)
target_link_libraries(${EXEC_NAME_TEST} PRIVATE cpptrace::cpptrace common_flags)
target_link_libraries(${EXEC_NAME_TEST} PRIVATE)

find_program(CLANG_TIDY_EXE NAMES clang-tidy)
find_program(CPPCHECK_EXE NAMES cppcheck)

if(CLANG_TIDY_EXE)
    message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
    set(CLANG_TIDY_COMMAND
        ${CLANG_TIDY_EXE}
        -quiet
        --config-file=${CMAKE_SOURCE_DIR}/.clang-tidy
        -p ${CMAKE_BINARY_DIR}
    )
    add_custom_target(clang-tidy
        COMMAND ${CLANG_TIDY_COMMAND} ${JAKARTA_SOURCES}
        COMMENT "Running clang-tidy..."
        USES_TERMINAL
        VERBATIM
    )

    set_target_properties(${EXEC_NAME} PROPERTIES C_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
    set_target_properties(${EXEC_NAME_TEST} PROPERTIES C_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
else()
    message(STATUS "clang-tidy not found")
endif()

if(CPPCHECK_EXE)
    message(STATUS "cppcheck found: ${CPPCHECK_EXE}")
    add_custom_target(cppcheck
        COMMAND ${CPPCHECK_EXE}
            --project=${CMAKE_BINARY_DIR}/compile_commands.json
            --file-filter=${CMAKE_SOURCE_DIR}/src/*
            --file-filter=${CMAKE_SOURCE_DIR}/include/*
            --enable=warning,style,performance,portability
            --suppress=missingIncludeSystem
            --inline-suppr
            --error-exitcode=1
            --quiet
            -j8
        COMMENT "Running cppcheck..."
        USES_TERMINAL
        VERBATIM
    )
else()
    message(STATUS "cppcheck not found")
endif()