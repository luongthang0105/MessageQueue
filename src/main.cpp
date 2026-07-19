#include <chrono>
#include <ctime>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "spdlog/spdlog.h"
#include <asio.hpp>
#include "server/TCPServer.hpp"

using asio::ip::tcp;

int main() {
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
