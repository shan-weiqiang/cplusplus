#include <iostream>
#include <mutex>
#include <thread>

/// since data is protected by different mutex data is not protected at all
void increment(std::mutex *m, int *data) {
  std::lock_guard<std::mutex> lck(*m);
  for (int i = 0; i < 10000; ++i) {
    ++(*data);
  }
}

int main() {
  int i{0};

  std::mutex m1_;
  std::mutex m2_;

  std::thread t1 = std::thread(increment, &m1_, &i);
  std::thread t2 = std::thread(increment, &m2_, &i);
  t1.join();
  t2.join();

  std::cout << i << std::endl;
}