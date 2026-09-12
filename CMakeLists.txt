cmake_minimum_required(VERSION 3.14)
project(CalcDemo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wformat -Werror")

# Использование FetchContent для подключения библиотеки calculat
include(FetchContent)
FetchContent_Declare(
  calculat
  GIT_REPOSITORY https://github.com/Daniil-Vkat/CALCULAT.git
  GIT_TAG        main
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(calculat)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE mathops)
