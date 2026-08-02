#pragma once
#include <unordered_map>
#include <utility>
#include <string>
#include "errors/MQErrors.h"
#include "MessageQueue/Topic.hpp"
class TopicManager {
    public:
        std::optional<MQErrors> create_topic(std::string_view topic_name) {
            bool insert_successful = topics.emplace(std::make_pair(topic_name, DefaultTopic {topic_name})).second;
            if (!insert_successful) {
                return MQErrors(MQErrorTypes::TopicAlreadyExist, MQErrorContext {.topic_name = std::string {topic_name}});
            }

            return std::nullopt;
        }

        std::optional<MQErrors> populate(std::string_view topic_name, std::string_view partition_key, DefaultPartitionItem item) {
            auto it = topics.find(std::string {topic_name});
            if (it == topics.end()) {
                return MQErrors(MQErrorTypes::TopicNotExist, MQErrorContext {.topic_name = std::string {topic_name}});
            }

            auto& topic = *it;
            topic.push_item(partition_key, item);

            return std::nullopt;
        }
    private:
        std::unordered_map<std::string, DefaultTopic> topics;
};