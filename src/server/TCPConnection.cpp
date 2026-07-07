#include "server/TCPConnection.hpp"
#include <ctime>
#include <string>

std::string make_daytime_string() {
    std::time_t now = std::time(0);
    return std::ctime(&now);
}

tcp::socket& TCPConnection::get_socket() { return socket_; }

void TCPConnection::start() {
    message_ = make_daytime_string();
    SPDLOG_INFO("Prepare sending message back to client");
    asio::async_write(socket_, asio::buffer(message_),
                        std::bind(&TCPConnection::handle_write, this,
                                std::placeholders::_1,
                                std::placeholders::_2));
}

void TCPConnection::handle_write(const asio::error_code &ec, size_t bytes_transferred) {
    SPDLOG_DEBUG("Try print ec: {}", ec.message());
    SPDLOG_INFO("I've sent {} bytes back!", bytes_transferred);
}