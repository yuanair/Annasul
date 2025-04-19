// C++ 20
// clang version 18.1.8
// Target: x86_64-pc-windows-msvc

#include <format>
#include <iostream>

int main() {
    auto num = 5;
    std::cout << std::format("{}", num) << std::endl;
    return 0;
}