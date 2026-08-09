#pragma once
#include <expected>
#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "errors/MQErrors.h"

template <typename T>
class Topic {
   public:
    using Partition = std::vector<T>;

    explicit Topic(std::string_view topic_name) : topic_name_{topic_name} {};

    /**
     * @brief Push an item to the end of a partition.
     */
    void push_item(std::string_view partition_key, T item) { partitions_[std::string{partition_key}].push_back(item); }

    /**
     * @brief Get an item from a partition, with an offset from the last item of
     * the partition.
     */
    std::expected<T, MQErrors> get_item(std::string_view partition_key, size_t offset) const {
        auto partition_it = partitions_.find(std::string{partition_key});
        if (partition_it == partitions_.end()) {
            return std::unexpected(MQErrors{MQErrorTypes::PartitionKeyNotExist,
                                            std::format("Partition of key \"{}\" does not exist.", partition_key)});
        }

        const Partition &partition = (*partition_it).second;
        if (offset >= partition.size()) {
            return std::unexpected(MQErrors{MQErrorTypes::OffsetExceedPartitionSize,
                                            std::format("Partition of key \"{}\" have size = {}, where offset = {}",
                                                        partition_key, partition.size(), offset)});
        }

        return partition[partition.size() - 1 - offset];
    }

    std::expected<T, MQErrors> get_last_item(std::string_view partition_key) const {
        return get_item(partition_key, 0);
    }

   private:
    std::string topic_name_;
    std::unordered_map<std::string, Partition> partitions_;
};

using DefaultPartitionItem = std::string;
using DefaultTopic = Topic<DefaultPartitionItem>;
