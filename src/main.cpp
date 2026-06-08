#include <chrono>
#include <ctime>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "spdlog/spdlog.h"
#include <asio.hpp>

using asio::ip::tcp;

constexpr uint16_t PORT_NUM = 10001;

std::string make_daytime_string() {
    std::time_t now = std::time(0);
    return std::ctime(&now);
}

class tcp_connection : std::enable_shared_from_this<tcp_connection> {
  public:
    typedef std::shared_ptr<tcp_connection> ptr;

    static ptr create_conn_ptr(asio::io_context &io_context) {
        return std::shared_ptr<tcp_connection>(new tcp_connection {io_context});
    }

    tcp::socket &get_socket() { return socket_; }
    void start() {
        message_ = make_daytime_string();
        SPDLOG_INFO("Prepare sending message back to client");
        asio::async_write(socket_, asio::buffer(message_),
                          std::bind(&tcp_connection::handle_write, this,
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
    explicit tcp_connection(asio::io_context &io_context) : socket_{io_context} {}
};

class tcp_server {
  private:
    asio::io_context io_context_{};
    tcp::acceptor acceptor_{io_context_, tcp::endpoint{tcp::v4(), PORT_NUM}};

    void start_accept() {
        tcp_connection::ptr conn_ptr =
            tcp_connection::create_conn_ptr(io_context_);

        acceptor_.async_accept(
            conn_ptr->get_socket(),
            std::bind(
                &tcp_server::handle_accept, this, conn_ptr,
                std::placeholders::_1));
    }

    void handle_accept(tcp_connection::ptr new_conn,
                       const asio::error_code &ec) {
        SPDLOG_INFO("Accepted a connection!");
        SPDLOG_DEBUG("Try print ec: {}", ec.message());
        if (!ec) {
            new_conn->start();
        }

        // calling this again to start accepting other connections as well
        start_accept();
    }

  public:
    tcp_server() {
        SPDLOG_INFO("Starting TCP Server!");
        start_accept();
        io_context_.run();
    }
    asio::io_context &get_io_context() { return io_context_; }
};

int main() {
    try {
        tcp_server server{};
    } catch (std::exception &e) {
        SPDLOG_ERROR("Exception: {}", e.what());
    }
    return 0;
}
