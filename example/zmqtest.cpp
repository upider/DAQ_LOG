#include "appender.hpp"
#include "loggerfactory.hpp"

using namespace daq;
int main(void)
{
    auto lf = LoggerFactory::instance();
    auto logger = lf->initialize("main");

    logger->addAppender(new ZMQAppender("tcp://192.168.43.141:9999"));
    logger->setOutputLevel(LogLevel::INFO);
    for (;;) {
        logger->info("zmqtest");
    }

    return 0;
}
