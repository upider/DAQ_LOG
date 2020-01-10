#include "loggerfactory.hpp"

using namespace DAQ;
int main()
{
    LoggerFactory::instance()->initFromFile("../example/configfiles/astest.json");
    auto loggers = LoggerFactory::instance()->getAllLoggerName();
    for (auto i : loggers) {
        std::cout << i << std::endl;
    }
    for (int i = 0; i < 100; ++i) {
        DLOG_INFO(LOCATIONINFO, "%s %d", "jjj", 100);
        DLOG_INFO(LOCATIONINFO, "%s %d", "jjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjsssssssssssssssssssjjj", 100);
    }
    return 0;
}
