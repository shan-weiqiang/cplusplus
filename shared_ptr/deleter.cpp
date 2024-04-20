#include <memory>
#include <iostream>
#include <vector>

struct SampleType {
    SampleType() {
        std::cout << "Constructor called\n";
    }

    ~SampleType() {
        std::cout << "Destructor called\n";
    }
};

int main() {
    std::shared_ptr<SampleType> ptr(new SampleType(), [](SampleType *p) {
        std::cout << "Custom deleter called\n";
        delete p;
    });
    std::shared_ptr<SampleType> normal_ptr(new SampleType());

    std::vector<std::shared_ptr<SampleType>> vec;
    vec.push_back(ptr);
    vec.push_back(normal_ptr);

    // ptr and normal_ptr are of the same type
    std::cout << "total:" << vec.size() << std::endl;
    return 0;
}