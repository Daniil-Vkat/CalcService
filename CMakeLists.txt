cmake_minimum_required(VERSION 3.14)
project(CalcDemo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# База данных компиляции для clang-tidy (build/compile_commands.json)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ------------------------------------------------------------------------------
# Опции инструментов качества кода (по умолчанию всё выключено)
# ------------------------------------------------------------------------------
option(ENABLE_CLANG_FORMAT        "Create clang-format targets (format, format-check)" OFF)
option(ENABLE_CLANG_TIDY          "Create clang-tidy targets (tidy, tidy-fix)"         OFF)
option(ENABLE_CLANG_TIDY_ON_BUILD "Run clang-tidy on every compilation of main"       OFF)

# ------------------------------------------------------------------------------
# Зависимость: библиотека calculat (таргет mathops)
# ------------------------------------------------------------------------------
include(FetchContent)
FetchContent_Declare(
  calculat
  GIT_REPOSITORY https://github.com/Daniil-Vkat/CALCULAT.git
  GIT_TAG        main
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(calculat)

# ------------------------------------------------------------------------------
# Основной таргет
# ------------------------------------------------------------------------------
add_executable(main main.cpp)
target_link_libraries(main PRIVATE mathops)
target_compile_options(main PRIVATE -Wall -Wextra -Wpedantic -Werror)

# Исходники, которые обрабатывают линтеры (зависимости из build/_deps не входят)
set(PROJECT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

# ------------------------------------------------------------------------------
# clang-format
# ------------------------------------------------------------------------------
if(ENABLE_CLANG_FORMAT)
    find_program(CLANG_FORMAT_EXE NAMES clang-format)
    if(NOT CLANG_FORMAT_EXE)
        message(FATAL_ERROR "ENABLE_CLANG_FORMAT=ON, but clang-format not found in PATH.")
    endif()
    message(STATUS "clang-format enabled: ${CLANG_FORMAT_EXE}")

    add_custom_target(format
        COMMAND ${CLANG_FORMAT_EXE} -i --style=file ${PROJECT_SOURCES}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "clang-format: formatting sources"
        VERBATIM
    )

    add_custom_target(format-check
        COMMAND ${CLANG_FORMAT_EXE} --dry-run --Werror --style=file ${PROJECT_SOURCES}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "clang-format: checking code style"
        VERBATIM
    )
endif()

# ------------------------------------------------------------------------------
# clang-tidy
# ------------------------------------------------------------------------------
if(ENABLE_CLANG_TIDY)
    find_program(CLANG_TIDY_EXE NAMES clang-tidy)
    if(NOT CLANG_TIDY_EXE)
        message(FATAL_ERROR "ENABLE_CLANG_TIDY=ON, but clang-tidy not found in PATH.")
    endif()
    message(STATUS "clang-tidy enabled: ${CLANG_TIDY_EXE}")

    add_custom_target(tidy
        COMMAND ${CLANG_TIDY_EXE} -p ${CMAKE_BINARY_DIR} ${PROJECT_SOURCES}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "clang-tidy: static analysis"
        VERBATIM
    )

    add_custom_target(tidy-fix
        COMMAND ${CLANG_TIDY_EXE} -p ${CMAKE_BINARY_DIR} --fix ${PROJECT_SOURCES}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "clang-tidy: analysis with auto-fix (modifies sources)"
        VERBATIM
    )

    # Встроить анализ в компиляцию только нашего таргета (mathops не затрагивается)
    if(ENABLE_CLANG_TIDY_ON_BUILD)
        set_target_properties(main PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
        message(STATUS "clang-tidy will run on every build of 'main'")
    endif()
elseif(ENABLE_CLANG_TIDY_ON_BUILD)
    message(WARNING "ENABLE_CLANG_TIDY_ON_BUILD=ON has no effect without ENABLE_CLANG_TIDY=ON")
endif()
