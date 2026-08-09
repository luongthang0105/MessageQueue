cmake_minimum_required(VERSION 3.27)
project(MQConsumer)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)

FetchContent_Declare(
    asio 
    GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
    GIT_TAG asio-1-38-0
)
FetchContent_MakeAvailable(asio)

add_executable(MQConsumer consumer.cpp)

target_include_directories(MQConsumer PRIVATE ${asio_SOURCE_DIR}/asio/include)
target_compile_definitions(MQConsumer PRIVATE ASIO_STANDALONE) # signals that we use standalone asio, not with boost