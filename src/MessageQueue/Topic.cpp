#include "errors/MQErrors.h"
#include "MessageQueue/Topic.hpp"

#include <exception>
#include <expected>
#include <format>
#include <string>

template <typename T>
void Topic<T>::push_item(std::string_view partition_key, T item) {
    partitions_[partition_key].push_back(item);
}

template <typename T>
std::expected<T, MQErrors> Topic<T>::get_item(std::string_view partition_key, size_t offset) const {
    auto partition_it = partitions_.find(partition_key);
    if (partition_it == partitions_.end()) {
        return std::unexpected(MQErrors {
            MQErrorTypes::PartitionKeyNotExist,
            std::format("Partition of key \"" {} "\" does not exist.", partition_key)
        });
    }

    Partition &partition = *partition_it;
    if (offset >= partition.size()) {
        return std::unexpected(MQErrors {
            MQErrorTypes::OffsetExceedPartitionSize,
            std::format("Partition of key \"" {} "\" have size = {}, where offset = ", partition_key, partition.size(), offset)
        });
    }

    return partition[partition.size() - offset];
}