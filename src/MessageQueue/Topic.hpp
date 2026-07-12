#pragma once
#include <string>
#include <unordered_map>
#include <vector>


template<typename T>
class Topic {
    public:
        using Partition = std::vector<T>;

        explicit Topic(std::string topicName): topicName_{topicName} {};

        /**
         * @brief Push an item to the end of a partition.
         */
        void push_item(std::string partitionKey, T item);

        /**
         * @brief Get an item from a partition, with an offset from the last item of the partition.
         */
        T get_item(std::string partitionKey, size_t offset) const;

        T get_last_item(std::string partitionKey) const {
            return get_item(partitionKey, 0);
        }

    private:
        std::string topicName_;
        std::unordered_map<std::string, Partition> partitions_;
};