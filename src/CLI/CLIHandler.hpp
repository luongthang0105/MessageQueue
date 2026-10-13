#pragma once
#include <format>
#include <iostream>
#include <optional>
#include <string>

#include "MessageQueue/TopicManager.hpp"

class CLIHandler {
   public:
    using Reply = std::string;

    static std::optional<Reply> handle(std::istream line_stream, TopicManager& topic_manager) {
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
                    reply = std::format("Error: {}", err.value().to_string());
                } else {
                    reply = std::format("Topic \"{}\" created successfully.", name);
                }
            } else if (operation == "populate") {
                std::string name;
                std::string partition_key;
                DefaultPartitionItem item;

                line_stream >> name >> partition_key >> item;
                if (const auto err = topic_manager.populate(name, partition_key, item)) {
                    reply = std::format("Error: {}", err.value().to_string());
                } else {
                    reply = std::format("Item added to topic \"{}\" at partition \"{}\"", name, partition_key);
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
            return std::nullopt;
        }
        return reply;
    }
};