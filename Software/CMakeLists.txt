cmake_minimum_required(VERSION 3.21)
project(TFG
    DESCRIPTION "Implementación del algoritmo de primalidad AKS"
    LANGUAGES CXX
)

# Speed-up recompilation with Ccache
find_program(CCACHE ccache)
if(CCACHE)
  set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
endif()

# Give the option to enable link-time optimizations
option(ENABLE_LTO "Enable link-time optimizations" ON)
include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT error)

if(ipo_supported AND ENABLE_LTO)
    message(STATUS "IPO / LTO enabled")
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
    message(STATUS "IPO / LTO not enabed: <${error}>")
endif()

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Release' as none was specified.")
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)

    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
        "Debug"
        "Release"
        "MinSizeRel"
        "RelWithDebInfo"
    )
endif()

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)

# standard compiler warnings
include(${PROJECT_SOURCE_DIR}/cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

# Link this 'library' to set the c++ standard / compile-time options requested
add_library(project_options INTERFACE)
# Use C++20 standar
target_compile_features(project_options INTERFACE cxx_std_20)

# allow for static analysis options
include(${PROJECT_SOURCE_DIR}/cmake/StaticAnalyzers.cmake)

# Download automatically, you can also just copy the conan.cmake file
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD
        "https://github.com/conan-io/cmake-conan/raw/v0.16.1/conan.cmake"
        "${CMAKE_BINARY_DIR}/conan.cmake"
        EXPECTED_HASH SHA256=396e16d0f5eabdc6a14afddbcfff62a54a7ee75c6da23f32f7a31bc85db23484
        TLS_VERIFY ON
    )
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)
conan_check(VERSION 1.40)

# Adding the bincrafters repository
conan_add_remote(
    NAME bincrafters
    URL https://bincrafters.jfrog.io/artifactory/api/conan/public-conan
)
# This is needed for the bincrafters
execute_process(COMMAND ${CONAN_CMD} config set general.revisions_enabled=1)

conan_cmake_autodetect(settings BUILD_TYPE Release)

conan_cmake_install(
    PATH_OR_REFERENCE ${PROJECT_SOURCE_DIR}
    BUILD missing
    SETTINGS ${settings}
)

include(${CMAKE_BINARY_DIR}/conan_paths.cmake)

find_package(gmp)
find_package(mpfr)

add_library(${PROJECT_NAME})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_sources(${PROJECT_NAME}
    # Public headers
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include/AKS.hpp
        ${PROJECT_SOURCE_DIR}/include/Basic.hpp
        ${PROJECT_SOURCE_DIR}/include/MillerRabin.hpp
        ${PROJECT_SOURCE_DIR}/include/SolovayStrassen.hpp
        ${PROJECT_SOURCE_DIR}/include/TFG.hpp
    # Private headers
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src/AKSCoefficient.hpp
        ${PROJECT_SOURCE_DIR}/src/AKSPolynomial.hpp
        ${PROJECT_SOURCE_DIR}/src/GMPWrappers.hpp
    # Implementation source files
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src/AKS.cpp
        ${PROJECT_SOURCE_DIR}/src/AKSCoefficient.cpp
        ${PROJECT_SOURCE_DIR}/src/AKSPolynomial.cpp
        ${PROJECT_SOURCE_DIR}/src/Basic.cpp
        ${PROJECT_SOURCE_DIR}/src/GMPWrappers.cpp
        ${PROJECT_SOURCE_DIR}/src/MillerRabin.cpp
        ${PROJECT_SOURCE_DIR}/src/SolovayStrassen.cpp
)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME}
    PUBLIC
        gmp::gmp
    PRIVATE
        mpfr::mpfr
        ntl
        project_options
        project_warnings
)

option(BUILD_EXAMPLES   "Builds examples"   OFF)
option(BUILD_TESTS      "Builds tests"      OFF)

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
