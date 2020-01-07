#include "loggerfactory.hpp"

int main() {
    AsLoggerFactory::instance()->initFromFile("/home/yzz/Documents/codes/CXX/sylar/log/tests/test.json");
    auto logger1 = AsLoggerFactory::instance()->initialize("ps");
    auto logger2 = AsLoggerFactory::instance()->initialize("ros");
    for (;;) {
        logger2->error("FASFJAS;flTEST", LOCATIONINFO);
    }
    return 0;
}
