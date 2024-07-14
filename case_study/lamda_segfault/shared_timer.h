#pragma once

#include "asio.hpp"
#include "asio/detail/chrono.hpp"
#include "asio/io_context.hpp"
#include "asio/steady_timer.hpp"
#include <atomic>
#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>

class timer : public std::enable_shared_from_this<timer> {
  public:
    timer(asio::io_context &io, uint64_t interval, std::function<void(void)> fn)
        : interval{interval}, fn_{fn},
          t{asio::steady_timer(io, asio::chrono::milliseconds(interval))} {}

    void start() {
        auto self{shared_from_this()};
        t.async_wait([self](std::error_code ec) { self->do_job(ec); });
    }

    void do_job(std::error_code ec) {

        // this is wrong design, since there is cycular reference to timer:
        // 1. timer is enclosed inside shared_ptr
        // 2. it's member asio::steady_timer t contains shared_ptr of timer, increment reference count
        // 3. it's member asio::steady_timer t must be destructed when timer is destructed, which is made impossible since asio::steady_timer t holds reference to timer
        if (shared_from_this().use_count() >= 3) {
            fn_();
            //   reset timer start timepoint
            t.expires_at(t.expiry() + asio::chrono::milliseconds(interval));
            auto self{shared_from_this()};
            t.async_wait([self](std::error_code ec) { self->do_job(ec); });
        } else {
            t.cancel();
        }
    }

    ~timer() {
        t.cancel();
    }

  private:
    uint64_t interval;
    std::function<void(void)> fn_;
    asio::steady_timer t;
};