#ifndef SRC_SERVER_TCPCONNECTION_HPP_
#define SRC_SERVER_TCPCONNECTION_HPP_

#include <asio.hpp>
#include <memory>
#include <string>

#include "MessageQueue/TopicManager.hpp"
#include "spdlog/spdlog.h"

using asio::ip::tcp;

std::string make_daytime_string();

class TCPConnection : public std::enable_shared_from_this<TCPConnection> {
   public:
    typedef std::shared_ptr<TCPConnection> ptr;

    static ptr create_conn_ptr(asio::io_context &io_context, std::shared_ptr<TopicManager> topic_manager) {
        return std::shared_ptr<TCPConnection>(new TCPConnection{io_context, topic_manager});
    }

    tcp::socket &get_socket();

    /**
     * @brief On connection, prepare messages and respond back to the client.
     */
    void start(TCPConnection::ptr conn_ptr);

   private:
    std::shared_ptr<TopicManager> topic_manager_;
    tcp::socket socket_;
    std::string message_;

    explicit TCPConnection(asio::io_context &io_context, std::shared_ptr<TopicManager> topic_manager)
        : socket_{io_context}, topic_manager_{topic_manager} {}
    void handle_read(std::shared_ptr<asio::streambuf> buffer, TCPConnection::ptr conn_ptr);
};

#endif  // SRC_SERVER_TCPCONNECTION_HPP_
