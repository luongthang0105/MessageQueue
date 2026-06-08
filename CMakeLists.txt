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

FetchContent_Declare(
    asio 
    GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
    GIT_TAG asio-1-38-0
)
FetchContent_MakeAvailable(asio)

add_executable(MessageQueue src/main.cpp)

target_link_libraries(MessageQueue PRIVATE spdlog::spdlog)

target_include_directories(MessageQueue PRIVATE ${asio_SOURCE_DIR}/asio/include)
target_compile_definitions(MessageQueue PRIVATE ASIO_STANDALONE) # signals that we use standalone asio, not with boost
