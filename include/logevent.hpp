#ifndef  __LOGEVENT_HPP_
#define  __LOGEVENT_HPP_

#include <cinttypes>
#include <thread>
#include <iostream>
#include <string>
#include <sstream>
#include <memory>
#include <sys/syscall.h>
#include <unistd.h>

#include <boost/fiber/all.hpp>

#include "locationinfo.hpp"
#include "loglevel.hpp"

#define _GNU_SOURCE


namespace daq {
/**
 * @brief 日志事件
 */
class LogEvent {
    public:
        using sptr = std::shared_ptr<LogEvent>;
        LogEvent();
        LogEvent(const std::string& LoggerName, LogLevel level,
                 const std::string& msg, const LocationInfo& locationInfo);
        ~LogEvent() {}
    public:
        const LogLevel getLevel() const {
            return m_level;
        }
        void setLevel(LogLevel level) {
            m_level = level;
        }
        static uint32_t getThreadId() {
            std::ostringstream oss;
            oss << syscall(__NR_gettid);
            uint32_t tid = std::stoull(oss.str());
            return tid;
        }
        static std::string getFiberId() {
            std::stringstream ss;
            ss << boost::this_fiber::get_id();
            return ss.str();
        }
        static uint64_t getTime() {
            return time(0);
        }
        const std::string getContent() const {
            return m_content;
        }
        std::string getLoggerName() const {
            return m_loggerName;
        }
        void setLoggerName(const std::string& name) {
            m_loggerName = name;
        }

        const char* getFileName() const {
            return m_locationInfo.getFileName();
        }
        int getLineNumber() const {
            return m_locationInfo.getLineNumber();
        }
        const std::string getClassName() const {
            return m_locationInfo.getClassName();
        }
        const std::string getMethodName() const {
            return m_locationInfo.getMethodName();
        }
    private:
        std::string m_loggerName = "root";       //logger名字
        LogLevel m_level = LogLevel::TRACE;      //日志级别
        std::string m_content;		             //日志内容
        const LocationInfo m_locationInfo;		 //位置信息
};

}
#endif /*__EVENT_HPP_ */
