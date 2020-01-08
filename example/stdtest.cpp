#include <chrono>
#include "loggerfactory.hpp"

int main() {
    auto lf = LoggerFactory::instance();
    auto logger = lf->initialize();
    logger->setFormatter("[%d{%Y-%m-%d %H:%M:%S}] [%p] [%f:%l] [%C] [%N] [%M] [%t] %m%n");

    logger->addAppender(new StdoutAppender);
    while (true) {
        //logger->info("StdoutTestfsfsf", LOCATIONINFO);
        logger->info("StdoutTestfsfsf");
    }

    return 0;
}
