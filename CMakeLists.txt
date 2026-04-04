cmake_minimum_required(VERSION 3.2)
project(MessageQueue)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.17.0
)

FetchContent_MakeAvailable(spdlog)

add_executable(MessageQueue main.cpp)
target_link_libraries(MessageQueue PRIVATE spdlog::spdlog)
