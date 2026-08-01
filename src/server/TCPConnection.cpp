#include "server/TCPConnection.hpp"
#include <ctime>
#include <iostream>
#include <string>
#include <utility>
#include "MessageQueue/TopicManager.hpp"

std::string make_daytime_string() {
    std::time_t now = std::time(0);
    return std::ctime(&now);
}

tcp::socket &TCPConnection::get_socket() { return socket_; }

void TCPConnection::start() {
    /**
     * \todo: show a list of commands here
     */
    SPDLOG_INFO("Welcome to MessageQueue!");

    TopicManager topic_manager{};
    while (true) {
        asio::streambuf buffer;

        asio::read_until(socket_, buffer, '\n');
        std::istream line_stream{&buffer};

        std::string reply;
        std::string command;
        line_stream >> command;

        if (command == "topic") {
            std::string operation;
            line_stream >> operation;

            if (operation == "create") {
                std::string name;
                line_stream >> name;

                if (!topic_manager.create_topic(name)) {
                    reply = std::format("Error: Topic \"{}\" existed.", name);
                } else {
                    reply = std::format(
                        "Succes: Topic \"{}\" created successfully.", name);
                }
            }
        } else if (command == "quit") {
            break;
        }

        asio::write(socket_, asio::buffer(reply));
        // asio::async_write(socket_, asio::buffer(message_),
        //                     std::bind(&TCPConnection::handle_write, this,
        //                             std::placeholders::_1,
        //                             std::placeholders::_2));
    }

    SPDLOG_INFO("Connection closed!");
}

void TCPConnection::handle_write(const asio::error_code &ec,
                                 size_t bytes_transferred) {
    SPDLOG_DEBUG("Try print ec: {}", ec.message());
    SPDLOG_INFO("I've sent {} bytes back!", bytes_transferred);
}