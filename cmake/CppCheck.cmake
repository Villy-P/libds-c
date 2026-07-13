option(DS_ENABLE_CPPCHECK "Enable cppcheck static analysis" OFF)

if(DS_ENABLE_CPPCHECK)
    find_program(CPPCHECK_EXE NAMES cppcheck)
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
endif()
