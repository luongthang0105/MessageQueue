#pragma once
#include <format>
#include <iostream>
#include <string>

enum class MQErrorTypes {
    TopicNotExist,
    TopicAlreadyExist,
};

struct MQErrorContext {
    std::string partition_key;
    std::string topic_name;
};

class MQErrors {
   public:
    MQErrors(MQErrorTypes type, MQErrorContext context) : type_{type}, context_{context} {};

    std::string_view to_string() const {
        switch (type_) {
            case MQErrorTypes::TopicAlreadyExist:
                return std::format("Topic \"{}\" already existed.", context_.topic_name);
            case MQErrorTypes::TopicNotExist:
                return std::format("Topic \"{}\" does not exist.", context_.topic_name);
            default:
                return "Unknown MQErrors.";
        }
    }

   private:
    MQErrorTypes type_;
    MQErrorContext context_;
};