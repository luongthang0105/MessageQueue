#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <sys/socket.h>
#include "spdlog/spdlog.h"

#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

std::vector<char> responseBuf(1024);
void ReadSome(asio::ip::tcp::socket &socket)
{
    socket.async_read_some(asio::buffer(responseBuf.data(), responseBuf.size()),
                           [&](const asio::error_code &ec, std::size_t bytesRead)
                           {
                               if (!ec)
                               {
                                   SPDLOG_INFO("Bytes read: {}", bytesRead);
                                   for (int i = 0; i < bytesRead; ++i)
                                   {
                                       std::cout << responseBuf[i];
                                   }

                                   ReadSome(socket);
                               }
                               else
                               {
                                   SPDLOG_INFO("Error message: {}", ec.message());
                               }
                           });
}
int main()
{
    asio::error_code ec;

    asio::io_context context;

    // fake work to keep context running
    auto workGuard = asio::make_work_guard(context);

    // let the context run on another thread
    std::thread thrdCtx{[&]()
                        { context.run(); }};

    asio::ip::tcp::endpoint endpoint{asio::ip::make_address("52.71.108.149", ec), 80};

    asio::ip::tcp::socket socket(context);

    socket.connect(endpoint, ec);

    if (!ec)
    {
        SPDLOG_INFO("Connected!\n");
    }
    else
    {
        std::cout << "Failed to connect to server: " << ec.message() << "\n";
        return 0;
    }

    if (socket.is_open())
    {
        ReadSome(socket);

        std::string request =
            "GET / HTTP/1.1\r\n"
            "Host: httpbin.org\r\n"
            "Connection: close\r\n\r\n";

        size_t bytes_written = socket.write_some(asio::buffer(request.data(), request.size()), ec);

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(10000ms);

        context.stop();
        if (thrdCtx.joinable())
        {
            thrdCtx.join();
        }
    }

    return 0;
}