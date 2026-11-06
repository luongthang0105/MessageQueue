#pragma once
#include <expected>
#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "MessageQueue/Partition/InMemoryPartitionManager.hpp"
#include "errors/MQErrors.h"

template <typename T, typename PartitionManager = InMemoryPartitionManager<T>>
class Topic {
   public:
    explicit Topic(std::string_view topic_name) : topic_name_{topic_name} {};

    /**
     * @brief Push an item to the end of a partition.
     */
    void push_item(std::string_view partition_key, T item) { partitions_.push_item(partition_key, item); }

    /**
     * @brief Get an item from a partition, with an offset from the last item of
     * the partition.
     */
    std::expected<T, MQErrors> get_item(std::string_view partition_key, size_t offset) const {
        return partitions_.get_item(partition_key, offset);
    }

    std::expected<T, MQErrors> get_last_item(std::string_view partition_key) const {
        return get_item(partition_key, 0);
    }

   private:
    std::string topic_name_;
    PartitionManager partitions_;
};

using DefaultPartitionItem = std::string;
using DefaultTopic = Topic<DefaultPartitionItem>;
