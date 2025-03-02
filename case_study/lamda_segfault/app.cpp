#include "asio.hpp"
#include "mock_class.h"
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

int main() {
    asio::io_context io;
    std::thread thread;

    {
        auto mock_ptr = std::make_shared<mock_class>();
        mock_ptr->start(io, 200);
        thread = std::thread([&io]() { io.run(); });
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    std::cout << "m1 and steady_timer inside is completely going out of scope" << std::endl;
    thread.join();
}