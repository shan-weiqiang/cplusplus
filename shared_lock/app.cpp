#include <atomic>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <vector>

static int cnt = 0;
std::shared_timed_mutex mtx;
std::atomic_bool started{false};
std::atomic_bool stopped{false};
std::condition_variable cv;
std::mutex cv_mutex;
std::atomic_int write_cnt{0};
std::atomic_int read_cnt{0};

struct TestType {
  int num;
};

void write() {
  {
    std::unique_lock<std::mutex> lck{cv_mutex};
    std::cout << "waiting to write" << std::endl;
    cv.wait(lck, []() -> bool { return started.load(); });
  }
  while (!stopped.load()) {

    {
      std::unique_lock<std::shared_timed_mutex> lock{mtx};
      std::cout << "write lock aquired" << std::endl;
      write_cnt++;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

void read() {
  {
    std::unique_lock<std::mutex> lck{cv_mutex};
    std::cout << "waiting to read" << std::endl;
    cv.wait(lck, []() -> bool { return started.load(); });
  }
  while (!stopped.load()) {
    {
      std::shared_lock<std::shared_timed_mutex> lock{mtx};
      std::cout << "reader shared lock aquired, by"
                << std::this_thread::get_id() << std::endl;
      read_cnt++;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

int main() {

  std::thread writer;
  std::vector<std::thread> readers;
  writer = std::thread(&write);
  for (int i = 0; i < 7; ++i) {
    readers.push_back(std::thread(&read));
  }

  started.store(true);
  cv.notify_all();

  std::this_thread::sleep_for(std::chrono::seconds(10));

  stopped.store(true);
  writer.join();
  for (auto &it : readers) {
    it.join();
  }

  std::cout << "write cnt: " << write_cnt << std::endl;
  std::cout << "read cnt: " << read_cnt << std::endl;
}