#pragma once
#include <format>
#include <iostream>
#include <string>

enum class MQErrorTypes {
    TopicNotExist,
    TopicAlreadyExist,
    PartitionKeyNotExist,
    OffsetExceedPartitionSize
};

class MQErrors {
   public:
    MQErrors(MQErrorTypes type, std::string message) : type_{type}, message_{message} {};

    std::string_view to_string() const {
        return message_;
    }

   private:
    MQErrorTypes type_;
    std::string message_;
};