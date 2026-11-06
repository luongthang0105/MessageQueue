#pragma once
#include <expected>
#include <string_view>

#include "errors/MQErrors.h"
template <typename T>
class IPartitionManager {
   public:
    virtual void push_item(std::string_view partition_key, T item) = 0;
    virtual std::expected<T, MQErrors> get_item(std::string_view partition_key, size_t offset) const = 0;

    virtual ~IPartitionManager() = default;
};