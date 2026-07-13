option(DS_ENABLE_CLANG_TIDY "Enable clang-tidy static analysis" OFF)

if(DS_ENABLE_CLANG_TIDY)
    find_program(CLANG_TIDY_EXE NAMES clang-tidy)
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
endif()