#include <array>
#include <iostream>
#include <asio.hpp>

using asio::ip::tcp;

int main(int argc, char *argv[]) {
    try {
        if (argc != 3) {
            std::cerr << "Usage: ./MQConsumer <host> <port>" << std::endl;
            return 1;
        }

        asio::io_context io_context;

        tcp::resolver resolver(io_context);
        tcp::resolver::results_type endpoints =
            resolver.resolve(argv[1], argv[2]);

        tcp::socket socket(io_context);
        asio::connect(socket, endpoints);

        for (;;)
        {
            std::string command;
            std::cin >> command;
            
            if (command == "quit") {
                socket.write_some(asio::buffer("quit"));
                goto close_socket;
            }

            asio::error_code error;

            socket.write_some(asio::buffer(command + '\n'), error);

            if (error) {
                throw std::system_error(error);
            }

            std::cout << "> ";

            std::array<char, 128> buf;
            size_t len = socket.read_some(asio::buffer(buf), error);

            if (error == asio::error::eof) {
                std::cout << "Connection closed cleanly by peer.";
                goto close_socket;
            } else if (error) {
                throw std::system_error(error); // Some other error.
            }

            std::cout.write(buf.data(), len);
        }
        close_socket:
            socket.close();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}