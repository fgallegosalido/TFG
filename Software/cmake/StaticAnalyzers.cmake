option(ENABLE_CLANG_TIDY "Enable static analysis with clang-tidy" OFF)
option(ENABLE_CPPCHECK "Enable static analysis with cppcheck" OFF)

if(ENABLE_CLANG_TIDY)
    find_program(CLANG_TIDY clang-tidy)
    if(CLANG_TIDY)
        set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY}
            --extra-arg=-Wno-unknown-warning-option
        )
    else()
        message(SEND_ERROR "clang-tidy requested but executable not found")
    endif()
endif()

if(ENABLE_CPPCHECK)
    find_program(CPPCHECK cppcheck)
    if(CPPCHECK)
        set(CMAKE_CXX_CPPCHECK ${CPPCHECK}
            --enable=warning,style
            --force
            --inconclusive
            --suppressions-list=${PROJECT_SOURCE_DIR}/suppressions.txt
        )
    else()
        message(SEND_ERROR "cppcheck requested but executable not found")
    endif()
endif()
