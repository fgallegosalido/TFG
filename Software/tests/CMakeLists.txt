find_package(Catch2)

add_executable(tfg_tests
    ${PROJECT_SOURCE_DIR}/tests/src/Main.cpp
    ${PROJECT_SOURCE_DIR}/tests/src/AKSTests.cpp
)

target_link_libraries(tfg_tests
    project_options
    project_warnings
    Catch2::Catch2
    TFG::TFG
)

add_test(NAME tfg_tests COMMAND tfg_tests)
