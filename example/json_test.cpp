#include <thread>
#include <chrono>
#include "loggerfactory.hpp"

int main(int argc, char* argv[]) {
    auto lf = LoggerFactory::instance();
    auto logger = lf->initialize();
    logger->setFormatter("[{\"headers\":{\"app_id\":\"%N\"},\"body\":\"[%d{%Y-%m-%d %H:%M:%S}] [%p] [%f:%l] [%C] [%M] [%t] %m\"}]");

    logger->addAppender(new StdoutAppender);
    logger->info("StdoutTestfsfsf", LOCATIONINFO);
    std::cout << std::endl;

    //std::this_thread::sleep_for(std::chrono::seconds(5));
    return 0;
}
