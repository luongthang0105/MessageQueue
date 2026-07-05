#ifndef SRC_SERVER_TCPCONNECTION_HPP_
#define SRC_SERVER_TCPCONNECTION_HPP_

#include <memory>
#include <asio.hpp>
#include "spdlog/spdlog.h"
#include "string"

using asio::ip::tcp;

std::string make_daytime_string() {
    std::time_t now = std::time(0);
    return std::ctime(&now);
}

class TCPConnection : std::enable_shared_from_this<TCPConnection> {
  public:
    typedef std::shared_ptr<TCPConnection> ptr;

    static ptr create_conn_ptr(asio::io_context &io_context) {
        return std::shared_ptr<TCPConnection>(new TCPConnection {io_context});
    }

    tcp::socket &get_socket() { return socket_; }
    void start() {
        message_ = make_daytime_string();
        SPDLOG_INFO("Prepare sending message back to client");
        asio::async_write(socket_, asio::buffer(message_),
                          std::bind(&TCPConnection::handle_write, this,
                                    std::placeholders::_1,
                                    std::placeholders::_2));
    }

    void handle_write(const asio::error_code &ec, size_t bytes_transferred) {
        SPDLOG_DEBUG("Try print ec: {}", ec.message());
        SPDLOG_INFO("I've sent {} bytes back!", bytes_transferred);
    }

  private:
    tcp::socket socket_;
    std::string message_;
    explicit TCPConnection(asio::io_context &io_context) : socket_{io_context} {}
};

#endif // SRC_SERVER_TCPCONNECTION_HPP_
