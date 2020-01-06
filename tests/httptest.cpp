#include "loggerfactory.hpp"

int main(void) {
    AsLoggerFactory::instance()->initFromFile("/home/yzz/Documents/codes/CXX/sylar/log/tests/http.json");
    auto logger1 = AsLoggerFactory::instance()->initialize("ps");

    for (int i = 0; i < 10; ++i) {
        logger1->info("hello logger", LOCATIONINFO);
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
    return 0;
}
