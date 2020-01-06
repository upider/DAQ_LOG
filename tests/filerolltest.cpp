#include <chrono>
#include "appender.hpp"
#include "loggerfactory.hpp"

int main(void)
{
    auto lf = LoggerFactory::instance();
    auto logger = lf->initialize("main");

    logger->addAppender(new RollFileAppender("./"));
    logger->setOutputLevel(LogLevel::INFO);
    logger->info("filerolltest");

    return 0;
}
