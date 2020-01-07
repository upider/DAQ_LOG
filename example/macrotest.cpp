#include "loggerfactory.hpp"

class A {
    public:
        A() {}
        ~A() {}
        void print() {
            for (int i = 0; i < 10; ++i) {
                DAQ_ASLOG_ERROR("%s", "hello");
            }
            []() {
                std::cout << __PRETTY_FUNCTION__ << std::endl;
            }
            ();
        }
};

int main()
{
    A a;
    AsLoggerFactory::instance()->initFromFile("/home/yzz/Documents/codes/CXX/sylar/log/tests/macro.json");
    a.print();
    []() {
        AsLoggerFactory::instance()->initialize("ps")->info("111111111111", LOCATIONINFO);
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }
    ();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return 0;
}
