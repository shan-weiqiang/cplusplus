#include <algorithm>
#include <array>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <vector>

int main() {
  std::vector<int> a = {1, 2, 3, 4};
  // elements themself are moved. not influencing the vector itself
  std::for_each(std::make_move_iterator(a.begin()),
                std::make_move_iterator(a.end()),
                [&](int &&e) { std::cout << e << std::endl; });
  // size is still 4
  std::cout << a.size() << std::endl;
  // vector itself is moved
  auto b = std::move(a);
  // size is 0
  std::cout << a.size() << std::endl;
}
