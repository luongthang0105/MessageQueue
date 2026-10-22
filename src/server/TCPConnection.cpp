#include "server/TCPConnection.hpp"

#include <ctime>
#include <iostream>
#include <string>
#include <utility>

#include "CLI/CLIHandler.hpp"
#include "MessageQueue/TopicManager.hpp"

std::string make_daytime_string() {
    std::time_t now = std::time(0);
    return std::ctime(&now);
}

tcp::socket& TCPConnection::get_socket() { return socket_; }

void TCPConnection::handle_read(std::shared_ptr<asio::streambuf> buffer, TCPConnection::ptr conn_ptr) {
    std::optional<CLIHandler::Reply> opt_reply = CLIHandler::handle(std::istream{buffer.get()}, *topic_manager_);

    if (!opt_reply.has_value()) {
        socket_.close();
        SPDLOG_INFO("Connection closed!");
        return;
    }

    socket_.async_write_some(asio::buffer(opt_reply.value()),
                             [conn_ptr](const asio::error_code& ec, std::size_t bytes_transfered) {
                                 if (!ec) {
                                     conn_ptr->start(conn_ptr);
                                 } else {
                                     SPDLOG_ERROR("Failed to write response to client: {}", ec.message());
                                 }
                             });
}

void TCPConnection::start(TCPConnection::ptr conn_ptr) {
    std::shared_ptr<asio::streambuf> buffer{std::make_shared<asio::streambuf>()};

    asio::async_read_until(socket_, *buffer.get(), '\n',
                           [conn_ptr, buffer](const asio::error_code& ec, std::size_t bytes_transfered) {
                               if (!ec) {
                                   conn_ptr->handle_read(buffer, conn_ptr);
                               } else {
                                   SPDLOG_ERROR("Failed to read input from client: {}", ec.message());
                               }
                           });
}