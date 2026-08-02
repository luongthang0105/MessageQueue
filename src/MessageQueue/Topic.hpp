#ifndef SRC_MESSAGEQUEUE_TOPIC_HPP_
#define SRC_MESSAGEQUEUE_TOPIC_HPP_

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

using DefaultPartitionItem = std::string;
using DefaultTopic = Topic<DefaultPartitionItem>;

template <typename T>
class Topic {
   public:
    using Partition = std::vector<T>;

    explicit Topic(std::string_view topic_name) : topic_name_{topic_name} {};

    /**
     * @brief Push an item to the end of a partition.
     */
    void push_item(std::string_view partition_key, T item);

    /**
     * @brief Get an item from a partition, with an offset from the last item of
     * the partition.
     */
    T get_item(std::string_view partition_key, size_t offset) const;

    T get_last_item(std::string_view partition_key) const { return get_item(partition_key, 0); }

   private:
    std::string topic_name_;
    std::unordered_map<std::string, Partition> partitions_;
};

#endif  // SRC_MESSAGEQUEUE_TOPIC_HPP_
