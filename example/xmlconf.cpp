#include "loggerfactory.hpp"

using namespace DAQ;
int main() {
    LoggerFactory::instance()->initFromFile("example/configfiles/test.xml");
    auto logger = LoggerFactory::instance()->initialize("ros");
    auto logger2 = LoggerFactory::instance()->initialize("ps");
    for (int i = 0; i < 100; i++) {
        logger->error("Hello ", LOCATIONINFO);
        logger2->error("World", LOCATIONINFO);
    }
    return 0;
}
