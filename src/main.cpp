#include <asio.hpp>
#include <chrono>
#include <ctime>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "server/TCPServer.hpp"
#include "spdlog/spdlog.h"

using asio::ip::tcp;

int main() {
    spdlog::set_level(spdlog::level::from_str(std::string {SPDLOG_LEVEL_NAME_DEBUG.data()}));

    try {
        TCPServer server{};
    } catch (std::exception &e) {
        SPDLOG_ERROR("Exception: {}", e.what());
    } catch (...) {
        /** \todo: try do some state saving here? */
        SPDLOG_ERROR("Unexpected exception caught!");
    }
    return 0;
}
