#include "asio.hpp"
#include "asio/io_context.hpp"
#include "timer.h"
#include <atomic>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <memory>
#include <mutex>
#include <sys/select.h>
#include <thread>
#include <vector>

class mock_class : public std::enable_shared_from_this<mock_class> {

  public:
    mock_class()
    {
    }

    void start(asio::io_context &io, uint64_t interval) {
        //  timer_(steady_timer) has handler that captures this pointer as a value copy and do_something will use members of this object;
        //   if timer_ is going out of scope, 'this' pointer captured is corrupted, calling of this handler will be undefined behavior

        //   when the expiry handler is called, both mock_class and timer object must be available, that said, timer must be fully disabled
        // BEFORE the destruction of timer and mock_class: inside the timer class, the timer life cycle is assured by weak_ptr; inside the
        //   mock_class it alse need a weak_ptr to assure the availability of mock_class when the expiry handler is called
        timer_ = std::make_shared<timer>(io, interval, [this, weak_ptr = std::weak_ptr<mock_class>(shared_from_this())]() {
            if (auto self = weak_ptr.lock()) {
                do_something();
            }
        });
        timer_->start();
    }
    ~mock_class() {
        // give some time before mock_class is destructed. This will be called before all members are destructed
        for (int i = 0; i < 10; ++i) {
            std::cout << i << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        std::cout << "mock class and members starting destruction..." << std::endl;
    }

    std::vector<std::string> names{"nihao", "shanghai"};
    std::shared_ptr<timer> timer_{nullptr};

    void do_something() {
        std::cout << "address of this: " << this << std::endl;
        for (const auto &ele : names) {
            std::cout << "address of ele: " << &ele << std::endl;
            for (int i = 0; i < ele.size(); i++) {
                std::cout << ele[i] << std::endl;
            }
            std::cout << ele << std::endl;
        }
    }
};