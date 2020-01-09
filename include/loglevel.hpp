#ifndef __LOGLEVEL_HPP_
#define __LOGLEVEL_HPP_

#include <string>

namespace DAQ {

//日志级别
enum class LogLevel {
    TRACE = 0,
    DEBUG = 1,
    INFO = 2,
    WARN = 3,
    ERROR = 4,
    FATAL = 5,
};

static std::string LoglevelToStr(LogLevel level) {
    switch(level) {
#define XX(name) \
    case LogLevel::name: \
        return #name; \
        break;

        XX(TRACE);
        XX(DEBUG);
        XX(INFO);
        XX(WARN);
        XX(ERROR);
        XX(FATAL);
#undef XX
    default:
        return "Unknow";
    }
    return "Unknow";
}

}
#endif /*__LOGLEVEL_HPP_*/
