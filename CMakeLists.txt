cmake_minimum_required(VERSION 3.10)
project(LIBDS_C VERSION 1.0)

set(EXEC_NAME_TEST "libds-c-test")
set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB_RECURSE LIBDS_C_TEST_SOURCES CONFIGURE_DEPENDS "test/*.c")
list(FILTER LIBDS_C_TEST_SOURCES EXCLUDE REGEX ".*src/main.c$")

include(cmake/Dependencies.cmake)
include(cmake/CompilerFlags.cmake)

add_executable(${EXEC_NAME_TEST} ${LIBDS_C_TEST_SOURCES})
target_include_directories(${EXEC_NAME_TEST} PRIVATE include/)
target_link_libraries(${EXEC_NAME_TEST} PRIVATE cpptrace::cpptrace common_flags)
target_link_libraries(${EXEC_NAME_TEST} PRIVATE)

find_program(CPPCHECK_EXE NAMES cppcheck)

include(cmake/ClangTidy.cmake)

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


add_custom_target(docs
    COMMAND doxygen ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating documentation"
    VERBATIM
)

include(cmake/AddLibrary.cmake)
include(cmake/ClangFormat.cmake)