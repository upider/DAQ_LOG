#include "loggerfactory.hpp"

using namespace DAQ;
int main() {
    AsLoggerFactory::instance()->initFromFile("./example/jsonfiles/astest.json");
    auto logger1 = AsLoggerFactory::instance()->initialize("ps");
    auto logger2 = AsLoggerFactory::instance()->initialize("ros");
    for (;;) {
        logger2->error("FASFJAS;flTEST", LOCATIONINFO);
    }
    return 0;
}
