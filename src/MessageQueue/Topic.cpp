#include <exception>
#include <string>
#include "MessageQueue/Topic.hpp"

template<typename T>
void Topic<T>::push_item(std::string partition_key, T item) {
    partitions_[partition_key].push_back(item);
}

template<typename T>
T Topic<T>::get_item(std::string partition_key, size_t offset) const {
    auto partition_it = partitions_.find(partition_key);
    if (partition_it == partitions_.end()) {
        std::string error_msg = "Partition of key \"" + partition_key + "\" does not exist."
        throw std::runtime_error(error_msg);
    }

    Partition& partition = *partition_it;
    if (offset >= partition.size()) {
        std::string error_msg = "Partition of key \"" + partition_key + "\" have size=" + partition.size()
                                + ", where offset=" + offset;
        throw std::runtime_error(error_msg);
    }

    return partition[partition.size() - offset];
}