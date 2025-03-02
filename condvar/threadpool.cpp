#include <cstddef>
#include <iostream>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <functional>
#include <thread>
#include <vector>

/// In this example the expression: stop || !tasks.empty() is the 'condition' between notifying
/// thread and waiting thread. The condition is used to avoid spurious wakeups. Every time the notifying
/// thread wants to do something that modifies the result of the condition, the mutex must be aquired.
/// The notify_one/notify_all can be called during the hold of the mutex, or can be called after the
/// release of the mutex, the difference is that if called during the hold of the lock, the waiting thread
/// will immediately blocked to wait for the release of the mutex by the calling thread here.

class ThreadPool {
  public:
    ThreadPool(std::size_t thread_num) : stop(false) {
        for (int i = 0; i < thread_num; i++) {
            threads.emplace_back(std::thread(&ThreadPool::thread_func, this));
        }
    }
    ~ThreadPool() {
        // aquire lock before changing the condition
        {
            std::unique_lock<std::mutex> lock(m);
            stop = true;
        }

        cv.notify_all();
        for (auto &thread : threads) {
            thread.join();
        }
    }
    template <typename F>
    void submit_task(F &&func) {
        // aquire lock before changing the condition
        {
            std::unique_lock<std::mutex> lock(m);
            tasks.emplace(std::forward<F>(func));
        }
        // call notify_all after release the lock; this will prevent the waiting thread from immediate block
        // after wake up
        cv.notify_one();
    }

  private:
    void thread_func() {
        while (true) {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lock(m);
                // wait for the condition to be true; condition is required. If no condtion, waiting thread
                // can not decide it's spurious wake-up or not
                cv.wait(lock, [this]() { return stop || !tasks.empty(); });
                if (stop && tasks.empty()) {
                    return;
                }
                // protected under the mutex, since these steps will change the condition
                task = std::move(tasks.front());
                tasks.pop();
            }
            // do not influence the condition, do it without holding the mutext
            task();
        }
    }
    std::queue<std::function<void()>> tasks;
    // This mutex is used for three purpose and these three purposes must be protected under this same mutex:
    // 1. Protect the tasks queue
    // 2. Protect the stop condition
    // 3. Protect the condition variable(which is a futex)
    // 1 and 2 both influence the result of the condition; 3 is required by the futex implementation in syscall
    std::mutex m;
    std::condition_variable cv;
    bool stop;
    std::vector<std::thread> threads;
};

int main() {
    // used to sync std::cout
    std::mutex m;
    ThreadPool pool(4);
    for (int i = 0; i < 8; i++) {
        pool.submit_task([i, &m]() {
            std::lock_guard<std::mutex> lock{m};
            std::cout << "Task " << i << " is running\n";
        });
    }
    std::this_thread::sleep_for(std::chrono::seconds(5));

    pool.submit_task([&m]() {
        std::lock_guard<std::mutex> lock{m};
        std::cout << "This is the last task" << std::endl;
    });

    return 0;
}
