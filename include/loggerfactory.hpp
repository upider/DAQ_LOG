#ifndef __LOGGERFACTORY_HPP_
#define __LOGGERFACTORY_HPP_

#include <string>
#include <map>
#include <boost/noncopyable.hpp>
#include "logconfig.hpp"
#include "logger.hpp"

namespace DAQ {

/// 同步Logger工厂
class LoggerFactory: public boost::noncopyable {
    public:
        using sptr = std::shared_ptr<LoggerFactory>;
        /// @brief instance 返回logger工厂实例
        ///
        /// @return LoggerFactory*
        static LoggerFactory* instance();
        /// @brief getAllLoggerName 返回已经有的logger的name
        ///
        /// @return loggerNames
        std::list<std::string> getAllLoggerName();

        /// @brief initialize 初始化日志logger
        ///
        /// @param name loggr名
        /// @param level 输出日志等级
        ///
        /// @return logger的智能指针
        Logger::sptr initialize(const std::string& name = "root", const LogLevel level = LogLevel::TRACE);
        /// @brief initFromFile 从文件初始化logger
        ///
        /// @param filename 文件名
        void initFromFile(const std::string& filename);
        Logger::sptr getFirstLogger() {
            for (auto e : m_loggers) {
                return e.second;
            }
            return m_loggers["root"];
        }

    private:
        static LoggerFactory* m_factory;
        std::mutex m_mutex;
        std::map<std::string, Logger::sptr> m_loggers;
        LogConfigurator* m_logConfer;

    private:
        LoggerFactory();
        virtual ~LoggerFactory();
};

/// 异步Logger工厂
class AsLoggerFactory : public boost::noncopyable {
    public:
        using sptr = std::shared_ptr<AsLoggerFactory>;
        /// @brief instance 返回异步logger工厂实例
        ///
        /// @return AsLoggerFactory*
        static AsLoggerFactory* instance();
        /// @brief getAllLoggerName 返回已经有的logger的name
        ///
        /// @return loggerNames
        std::list<std::string> getAllLoggerName();
        /// @brief initialize 初始化日志logger
        ///
        /// @param name loggr名
        /// @param level 输出日志等级
        /// @param size 异步buffer大小
        ///
        /// @return 异步logger的智能指针
        AsLogger::sptr initialize(const std::string& name = "root",
                                  const LogLevel level = LogLevel::TRACE,
                                  size_t size = 256);
        /// @brief initFromFile 从文件初始化logger
        ///
        /// @param filename 文件名
        void initFromFile(const std::string& filename);
        Logger::sptr getFirstLogger() {
            for (auto e : m_loggers) {
                return e.second;
            }
            return m_loggers["root"];
        }
    private:
        static AsLoggerFactory* m_factory;
        LogConfigurator* m_logConfer;
        std::mutex m_mutex;
        std::map<std::string, AsLogger::sptr> m_loggers;
    private:
        AsLoggerFactory();
        virtual ~AsLoggerFactory ();
};

//log宏，将log输出到控制台
/*******************************************************************************/
#define DAQ_LOG_TRACE(fmt, ...) {\
	if ((fmt)) { \
		char log_buffer_[sizeof(__VA_ARGS__)];\
		sprintf(log_buffer_, fmt, __VA_ARGS__); \
		LoggerFactory::instance()->getFirstLogger()->trace(log_buffer_,LOCATIONINFO);\
	}\
}

#define DAQ_LOG_INFO(fmt, ...) {\
	if ((fmt)) { \
		char log_buffer_[sizeof(__VA_ARGS__)];\
		sprintf(log_buffer_, fmt, __VA_ARGS__); \
		LoggerFactory::instance()->getFirstLogger()->info(log_buffer_,LOCATIONINFO);\
	}\
}

#define DAQ_LOG_DEBUG(fmt, ...) {\
	if ((fmt)) { \
		char log_buffer_[sizeof(__VA_ARGS__)];\
		sprintf(log_buffer_, fmt, __VA_ARGS__); \
		LoggerFactory::instance()->getFirstLogger()->debug(log_buffer_,LOCATIONINFO);\
	}\
}

#define DAQ_LOG_ERROR(fmt, ...) {\
	if ((fmt)) { \
		char log_buffer_[sizeof(__VA_ARGS__)];\
		sprintf(log_buffer_, fmt, __VA_ARGS__); \
		LoggerFactory::instance()->getFirstLogger()->error(log_buffer_,LOCATIONINFO);\
	}\
}

#define DAQ_LOG_FATAL(fmt, ...) {\
	if ((fmt)) { \
		char log_buffer_[sizeof(__VA_ARGS__)];\
		sprintf(log_buffer_, fmt, __VA_ARGS__); \
		LoggerFactory::instance()->getFirstLogger()->fatal(log_buffer_,LOCATIONINFO);\
	}\
}

#define DAQ_ASLOG_TRACE(fmt, ...) {\
	if ((fmt)) { \
		char log_buffer_[sizeof(__VA_ARGS__)];\
		sprintf(log_buffer_, fmt, __VA_ARGS__); \
		AsLoggerFactory::instance()->getFirstLogger()->trace(log_buffer_,LOCATIONINFO);\
	}\
}

#define DAQ_ASLOG_INFO(fmt, ...) {\
	if ((fmt)) { \
		char log_buffer_[sizeof(__VA_ARGS__)];\
		sprintf(log_buffer_, fmt, __VA_ARGS__); \
		AsLoggerFactory::instance()->getFirstLogger()->info(log_buffer_,LOCATIONINFO);\
	}\
}

#define DAQ_ASLOG_DEBUG(fmt, ...) {\
	if ((fmt)) { \
		char log_buffer_[sizeof(__VA_ARGS__)];\
		sprintf(log_buffer_, fmt, __VA_ARGS__); \
		AsLoggerFactory::instance()->getFirstLogger()->debug(log_buffer_,LOCATIONINFO);\
	}\
}

#define DAQ_ASLOG_ERROR(fmt, ...) {\
	if ((fmt)) { \
		char log_buffer_[sizeof(__VA_ARGS__)];\
		sprintf(log_buffer_, fmt, __VA_ARGS__); \
		AsLoggerFactory::instance()->getFirstLogger()->error(log_buffer_,LOCATIONINFO);\
	}\
}

#define DAQ_ASLOG_FATAL(fmt, ...) {\
	if ((fmt)) { \
		char log_buffer_[sizeof(__VA_ARGS__)];\
		sprintf(log_buffer_, fmt, __VA_ARGS__); \
		AsLoggerFactory::instance()->getFirstLogger()->fatal(log_buffer_,LOCATIONINFO);\
	}\
}
/*******************************************************************************/

}
#endif /* __LOGGERFACTORY_HPP_ */
