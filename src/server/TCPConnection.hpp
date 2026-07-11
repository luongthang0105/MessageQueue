#ifndef SRC_SERVER_TCPCONNECTION_HPP_
#define SRC_SERVER_TCPCONNECTION_HPP_

#include <memory>
#include <string>

#include <asio.hpp>
#include "spdlog/spdlog.h"

using asio::ip::tcp;

std::string make_daytime_string();

class TCPConnection : std::enable_shared_from_this<TCPConnection> {
  public:
    typedef std::shared_ptr<TCPConnection> ptr;

    static ptr create_conn_ptr(asio::io_context &io_context) {
        return std::shared_ptr<TCPConnection>(new TCPConnection {io_context});
    }

    tcp::socket& get_socket();

    /**
     * @brief On connection, prepare messages and respond back to the client.
     */
    void start();

    void handle_write(const asio::error_code &ec, size_t bytes_transferred);

  private:
    tcp::socket socket_;
    std::string message_;
    explicit TCPConnection(asio::io_context &io_context) : socket_{io_context} {}
};

#endif // SRC_SERVER_TCPCONNECTION_HPP_
