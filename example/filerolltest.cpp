#include <chrono>
#include "appender.hpp"
#include "loggerfactory.hpp"

using namespace DAQ;
int main(void)
{
    auto lf = LoggerFactory::instance();
    auto logger = lf->initialize("main");

    logger->addAppender(new RollFileAppender("/home/yzz/Documents/codes/CXX/log/build/test_logs", 8, "hello", "world"));
    logger->setOutputLevel(LogLevel::INFO);
    for (int i = 0; i < 100; ++i) {
        //logger->info("filerolltest", LOCATIONINFO);
        logger->info("filerolltest");
        //std::cout << i << std::endl;
    }

    return 0;
}
