#include "loggerfactory.hpp"

using namespace daq;
int main(void) {
    AsLoggerFactory::instance()->initFromFile("./example/jsonfiles/http.json");
    auto logger1 = AsLoggerFactory::instance()->initialize("ps");

    for (int i = 0; i < 10; ++i) {
        logger1->info("hello logger", LOCATIONINFO);
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
    return 0;
}
