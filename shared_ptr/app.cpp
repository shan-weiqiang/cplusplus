
#include <iostream>
#include <memory>

class sample_class {
public:
  sample_class() { std::cout << "constructor called~\n"; }
};

int main() {
  auto instance = std::make_shared<sample_class>();
  std::cout << instance.use_count() << std::endl;
  std::shared_ptr<sample_class> &ref = instance;
  std::shared_ptr<sample_class> &ref2 = instance;
  std::cout << instance.use_count() << std::endl;
}
