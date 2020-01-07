#ifndef  __LOGEVENT_HPP_
#define  __LOGEVENT_HPP_

#include <cinttypes>
#include <thread>
#include <iostream>
#include <string>
#include <sstream>
#include <memory>
#include <boost/fiber/all.hpp>
#include "locationinfo.hpp"
#include "loglevel.hpp"

/*
 * @brief 构造函数
 * @param[in] logger 日志器
 * @param[in] level 日志级别
 * @param[in] file 文件名
 * @param[in] line 文件行号
 * @param[in] elapse 程序启动依赖的耗时(毫秒)
 * @param[in] thread_id 线程id
 * @param[in] fiber_id 协程id(boost.fiber)
 * @param[in] time 日志事件(秒)
 * @param[in] thread_name 线程名称
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
        //uint32_t getElapse() const {
        //    return m_elapse;
        //}
        static uint32_t getThreadId() {
            std::ostringstream oss;
            oss << getpid();
            //oss << std::this_thread::get_id();
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
        //uint32_t m_elapse = 0;                   //程序启动开始到现在的毫秒数
        //uint32_t m_threadId = 0;                 //线程ID
        //uint32_t m_fiberId = 0;                  //协程ID
        //uint64_t m_time = time(0);               //开始时间时间
        std::string m_content;		             //日志内容
        const LocationInfo m_locationInfo;		 //位置信息
};

#endif /*__EVENT_HPP_ */