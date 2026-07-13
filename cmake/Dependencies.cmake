include(FetchContent)
fetchcontent_declare(cpptrace
    GIT_REPOSITORY https://github.com/jeremy-rifkin/cpptrace.git
    GIT_TAG        v0.7.5
)
fetchcontent_makeavailable(cpptrace)

FetchContent_Declare(doxygen-awesome-css
    URL https://github.com/jothepro/doxygen-awesome-css/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(doxygen-awesome-css)
set(AWESOME_CSS_DIR ${doxygen-awesome-css_SOURCE_DIR})
file(COPY ${AWESOME_CSS_DIR}/doxygen-awesome.css
     DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/docs/theme)

message(STATUS "Doxygen Awesome CSS: ${AWESOME_CSS_DIR}")