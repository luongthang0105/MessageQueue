#pragma once
#include <expected>
#include <functional>
#include <string>
#include <unordered_map>
#include <utility>

#include "MessageQueue/Topic.hpp"
#include "errors/MQErrors.h"
class TopicManager {
   public:
    std::optional<MQErrors> create_topic(std::string_view topic_name) {
        bool insert_successful = topics.emplace(std::make_pair(topic_name, DefaultTopic{topic_name})).second;
        if (!insert_successful) {
            return MQErrors(MQErrorTypes::TopicAlreadyExist, std::format("Topic \"{}\" already existed.", topic_name));
        }

        return std::nullopt;
    }

    std::optional<MQErrors> populate(std::string_view topic_name, std::string_view partition_key,
                                     DefaultPartitionItem item) {
        if (auto exp_topic = get_topic(topic_name)) {
            exp_topic.value().get().push_item(partition_key, item);
            return std::nullopt;
        } else {
            // \todo: this can cause performance issue because we pass MQErrors by value
            return exp_topic.error();
        }
        
    }

    std::expected<std::string, MQErrors> consume(std::string_view topic_name, std::string_view partition_key,
                                                 size_t offset) {
        if (auto exp_topic = get_topic(topic_name)) {
            return exp_topic.value().get().get_item(partition_key, offset);
        } else {
            // \todo: this can cause performance issue because we pass MQErrors by value
            return std::unexpected(exp_topic.error());
        }
    }

   private:
    std::unordered_map<std::string, DefaultTopic> topics;

    std::expected<std::reference_wrapper<DefaultTopic>, MQErrors> get_topic(std::string_view topic_name) {
        auto it = topics.find(std::string{topic_name});
        if (it == topics.end()) {
            return std::unexpected(MQErrors{
                MQErrorTypes::TopicNotExist, 
                std::format("Topic \"{}\" does not exist.", topic_name)
            });
        }
        return (*it).second;
    }
};