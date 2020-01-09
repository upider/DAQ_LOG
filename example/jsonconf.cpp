#include "loggerfactory.hpp"

using namespace DAQ;
int main() {
    AsLoggerFactory::instance()->initFromFile("example/jsonfiles/astest.json");
    auto logger = AsLoggerFactory::instance()->initialize("ros");
    for (;;) {
        logger->error("FASFJAS;flTEST", LOCATIONINFO);
    }
    return 0;
}
