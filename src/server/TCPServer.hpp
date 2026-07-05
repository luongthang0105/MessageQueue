#ifndef SRC_SERVER_TCPSERVER_HPP_
#define SRC_SERVER_TCPSERVER_HPP_

#include <memory>
#include <string>

#include "spdlog/spdlog.h"
#include <asio.hpp>
#include "src/server/TCPConnection.hpp"

using asio::ip::tcp;

constexpr uint16_t PORT_NUM = 10001;

class TCPServer {
  private:
    asio::io_context io_context_{};
    tcp::acceptor acceptor_{io_context_, tcp::endpoint{tcp::v4(), PORT_NUM}};

    void start_accept() {
        TCPConnection::ptr conn_ptr =
            TCPConnection::create_conn_ptr(io_context_);

        acceptor_.async_accept(
            conn_ptr->get_socket(),
            std::bind(
                &TCPServer::handle_accept, this, conn_ptr,
                std::placeholders::_1));
    }

    void handle_accept(TCPConnection::ptr new_conn,
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
    TCPServer() {
        SPDLOG_INFO("Starting TCP Server!");
        start_accept();
        io_context_.run();
    }
    asio::io_context &get_io_context() { return io_context_; }
};

#endif // SRC_SERVER_TCPSERVER_HPP_