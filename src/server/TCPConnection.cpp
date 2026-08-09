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

                if (const auto err = topic_manager.create_topic(name)) {
                    reply = std::format("Topic \"{}\" created successfully.", name);
                } else {
                    reply = std::format("Error: {}", err.value().to_string());
                }
            } else if (operation == "populate") {
                std::string name;
                std::string partition_key;
                DefaultPartitionItem item;

                line_stream >> name >> partition_key >> item;
                if (const auto err = topic_manager.populate(name, partition_key, item)) {
                    reply = std::format("Item added to topic \"{}\" at partition \"{}\"", name, partition_key);
                } else {
                    reply = std::format("Error: {}", err.value().to_string());
                }
            } else if (operation == "consume") {
                std::string name;
                std::string partition_key;
                size_t offset;

                line_stream >> name >> partition_key >> offset;
                if (const auto exp_item = topic_manager.consume(name, partition_key, offset)) {
                    reply = std::format("{}", exp_item.value());
                } else {
                    reply = std::format("Error: {}", exp_item.error().to_string());
                }
            }
        } else if (command == "quit") {
            break;
        }
    }

    SPDLOG_INFO("Connection closed!");
}