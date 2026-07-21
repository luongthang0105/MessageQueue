#pragma once
#include <unordered_map>
#include <utility>
#include <string>
#include "MessageQueue/Topic.hpp"

class TopicManager {
    public:
        bool create_topic(std::string_view topic_name) {
            bool insert_successful = topics.emplace(std::make_pair(topic_name, DefaultTopic {topic_name})).second;
            return insert_successful;
        }
    private:
        using DefaultTopic = Topic<std::string>;
        std::unordered_map<std::string, DefaultTopic> topics;
};