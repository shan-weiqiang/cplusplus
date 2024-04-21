#include <memory>
#include <iostream>

class Incomplete;

int main() {
    Incomplete *raw;
    std::shared_ptr<Incomplete> share_ptr(raw);
    std::unique_ptr<Incomplete> unique_ptr(raw);
}