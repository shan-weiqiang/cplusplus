#pragma once

#include "asio.hpp"
#include "asio/detail/chrono.hpp"
#include "asio/io_context.hpp"
#include "asio/steady_timer.hpp"
#include <atomic>
#include <chrono>
#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>
#include <thread>

// encapculate steady_timer to make a triggering timer with specified time interval
class timer : public std::enable_shared_from_this<timer> {
  public:
    timer(asio::io_context &io, uint64_t interval, std::function<void(void)> fn)
        : interval{interval}, fn_{fn},
          t{asio::steady_timer(io, asio::chrono::milliseconds(interval))} {}

    void start() {
        t.async_wait([this, wptr = std::weak_ptr<timer>{shared_from_this()}](std::error_code ec) {
            // this has to be used together with t.cancel(); if this error code is not processed, cancel() have no effect
            // Note that since this might be called after the complete destruction of 'this', the handle of asio::error::operation_aborted
            // should not use any resource of 'this'
            if (ec == asio::error::operation_aborted) {
                std::cout << "io context called aborted timer handler" << std::endl;
                return;
            } else {
                // lock resources that requried by do_job, note that do_job contains two kind of resources:
                // 1. interval, t: thoese are members totally controlled by timer, wptr.lock() operation can assure that they are valid
                // 2. fn_: this is registered from the user of timer, since timer is used as shared_ptr, timer itself can not assure that
                // fn_ is still valid even when timer itself is valid. For example, it might happen that during the call of fn_, the resources
                // used by this callable(for example, values captured by lambda) is being/already dealloocated. Normally timer itself and the
                // captured value/variable are managed by the same object, but since the wptr.lock(), the timer is alive after it is destructed
                // as a shared_ptr in the out level object.

                //  To be clear: for the expiry handler, to keep it valid, every lambda function that are registed should manage it's own resources
                // and be certain that during the execution of this handler, there must be no invalid resources
                if (auto self = wptr.lock()) {
                    do_job(ec);
                }
            }
        });
    }

    void do_job(std::error_code ec) {

        // if asio::error::operation_aborted is not processed following code will be executed by io context, even
        // after asio::steady_timer t is completely destructed, which is undefined behavior.
        // Becasue fn_/do_job are both captured inside a lamda expression, which will be stored inside asio::steady_timer t
        // as callable object, when this callable object is disposed and being called after disposition, undefined behavior
        // happens

        // do_job execution depends on the existence of 'this', but should not influence the life cycle of 'this'

        fn_();
        //   reset timer start timepoint
        t.expires_at(t.expiry() + asio::chrono::milliseconds(interval));
        t.async_wait([this, wptr = std::weak_ptr<timer>{shared_from_this()}](std::error_code ec) {if (ec == asio::error::operation_aborted) {
                std::cout << "io context called aborted timer handler" << std::endl;
                return;
            } else {
                // lock resources that requried by do_job
                if (auto self = wptr.lock()) {
                    do_job(ec);
                }
            } });
    }

    ~timer() {
        // calls waiting handlers with asio::error::operation_aborted
        // ! only affect async_wait handlers BEFORE the call; if async_wait called after cancel(), it still will be executed as normal expiry
        t.cancel();
    }

  private:
    uint64_t interval;
    std::function<void(void)> fn_;
    // when t is destructed, it will de-register the i/o monitoring from the io context
    asio::steady_timer t;
};