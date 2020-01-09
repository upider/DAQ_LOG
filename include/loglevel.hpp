#ifndef __LOGLEVEL_HPP_
#define __LOGLEVEL_HPP_

#include <string>

namespace DAQ {

/// @brief 日志级别，对应数字为0-5
enum class LogLevel {
    TRACE = 0,
    DEBUG = 1,
    INFO = 2,
    WARN = 3,
    ERROR = 4,
    FATAL = 5,
};

/// @brief LoglevelToStr 将日志等级转化为string
///
/// @param level 日志等级
///
/// @return 日志等级string
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
