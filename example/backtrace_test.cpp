#include <iostream>
#include "backtrace.hpp"

void test() {
    std::cout << "hello" << std::endl;
    std::vector<std::string> bt;
    std::cout <<  BacktraceToString(10, 1, "") << std::endl;
}

int main()
{
    test();
    std::cout << "world" << std::endl;

    return 0;
}
