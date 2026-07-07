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
  public:
    TCPServer();
    asio::io_context &get_io_context() { return io_context_; }

  private:
    asio::io_context io_context_{};
    tcp::acceptor acceptor_{io_context_, tcp::endpoint{tcp::v4(), PORT_NUM}};

    /**
     * @brief Creates a TCP socket, listening to new connections. Calls @ref TCPServer::handle_accept() after accepting a new connection.
     */
    void start_accept();

    void handle_accept(TCPConnection::ptr new_conn, const asio::error_code &ec);
};

#endif // SRC_SERVER_TCPSERVER_HPP_