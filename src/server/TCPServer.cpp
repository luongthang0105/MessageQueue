#include "server/TCPServer.hpp"

void TCPServer::start_accept() {
    SPDLOG_INFO("Open to accepting new connections.");

    TCPConnection::ptr conn_ptr = TCPConnection::create_conn_ptr(io_context_);

    acceptor_.async_accept(conn_ptr->get_socket(),
                           std::bind(&TCPServer::handle_accept, this, conn_ptr, std::placeholders::_1));
}

void TCPServer::handle_accept(TCPConnection::ptr new_conn, const asio::error_code &ec) {
    SPDLOG_INFO("Accepted a connection!");

    if (!ec) {
        new_conn->start();
    }

    // calling this again to start accepting other connections as well
    start_accept();
}

TCPServer::TCPServer() {
    SPDLOG_INFO("Starting TCP Server at port {}!", TCPServer::PORT_NUM);
    start_accept();
    io_context_.run();
}