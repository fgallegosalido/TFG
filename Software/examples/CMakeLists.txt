find_package(benchmark)
find_package(fmt)
find_package(magic_enum)

add_executable(Main)
target_sources(Main
    PRIVATE
        ${PROJECT_SOURCE_DIR}/examples/src/Main.cpp
)
target_link_libraries(Main
    project_options
    project_warnings
    fmt::fmt
    magic_enum::magic_enum
    TFG::TFG
)

add_executable(Benchmarks)
target_sources(Benchmarks
    PRIVATE
        ${PROJECT_SOURCE_DIR}/examples/src/Benchmarks.cpp
)
target_link_libraries(Benchmarks
    project_options
    project_warnings
    benchmark::benchmark
    fmt::fmt
    TFG::TFG
)
