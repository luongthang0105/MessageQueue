#pragma once
#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "MessageQueue/Partition/IPartitionManager.hpp"
#include "errors/MQErrors.h"

template <typename T>
class InMemoryPartitionManager : public IPartitionManager<T> {
   public:
    void push_item(std::string_view partition_key, T item) override { partitions_[std::string{partition_key}].push_back(item); }

    std::expected<T, MQErrors> get_item(std::string_view partition_key, size_t offset) const override {
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

   private:
    using Partition = std::vector<T>;
    std::unordered_map<std::string, Partition> partitions_;
};