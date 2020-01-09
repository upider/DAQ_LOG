#include "loggerfactory.hpp"

using namespace DAQ;
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
    AsLoggerFactory::instance()->initFromFile("./example/jsonfiles/astest.json");
    a.print();
    []() {
        AsLoggerFactory::instance()->initialize("ps")->info("111111111111", LOCATIONINFO);
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }
    ();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return 0;
}
