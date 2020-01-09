#include "loggerfactory.hpp"

using namespace DAQ;
int main() {
    auto lf = LoggerFactory::instance();
    auto logger = lf->initialize();
    logger->setFormatter("[%d{%Y-%m-%d %H:%M:%S}] [%p] [%f:%l] [%C] [%N] [%M] [%t] %m%n");

    logger->addAppender(new SingleFileAppender("/home/yzz/Documents/codes/CXX/log/build/test.txt"));
    for (int i = 0; i < 100; ++i) {
        logger->info("Hello", LOCATIONINFO);
        logger->info("World!");
    }

    return 0;
}
