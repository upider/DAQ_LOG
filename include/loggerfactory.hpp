#ifndef __LOGGERFACTORY_HPP_
#define __LOGGERFACTORY_HPP_

#include <string>
#include <map>
#include <boost/noncopyable.hpp>
#include "logconfig.hpp"
#include "logger.hpp"

namespace daq {

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
	if ((fmt)) {\
		char log_buffer_[sizeof(__VA_ARGS__)];\
		sprintf(log_buffer_, fmt, __VA_ARGS__);\
		LoggerFactory::instance()->getFirstLogger()->info(log_buffer_, LOCATIONINFO);\
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

//LoggerFactory
template<typename... Args>
/**
 * @brief DLOG_TRACE 打印trace-log
 *
 * @param locationInfo 位置信息
 * @param fmt 格式
 * @param args 可变参数
 */
inline void DLOG_TRACE(const LocationInfo& locationInfo, const std::string& fmt, Args... args) {
    constexpr size_t old_len = 128;
    char old_buffer[old_len];
    size_t new_len = snprintf(old_buffer, old_len, fmt.c_str(), args...);
    // 算上终止符'\0'
    new_len++;

    if (new_len > old_len) {
        std::vector<char> new_buffer(new_len);
        snprintf(new_buffer.data(), new_len, fmt.c_str(), args...);
        return LoggerFactory::instance()->getFirstLogger()->trace(new_buffer.data(), locationInfo);
    }

    return LoggerFactory::instance()->getFirstLogger()->trace(old_buffer, locationInfo);
}

template<typename... Args>
/**
 * @brief DLOG_TRACE 打印trace-log
 *
 * @param fmt 格式
 * @param args 可变参数
 */
inline void DLOG_TRACE(const std::string& fmt, Args... args) {
    constexpr size_t old_len = 128;
    char old_buffer[old_len];
    size_t new_len = snprintf(old_buffer, old_len, fmt.c_str(), args...);
    // 算上终止符'\0'
    new_len++;

    if (new_len > old_len) {
        std::vector<char> new_buffer(new_len);
        snprintf(new_buffer.data(), new_len, fmt.c_str(), args...);
        return LoggerFactory::instance()->getFirstLogger()->trace(new_buffer.data());
    }

    return LoggerFactory::instance()->getFirstLogger()->trace(old_buffer);
}

template<typename... Args>
/**
 * @brief DLOG_INFO 打印info-log
 *
 * @param locationInfo 位置信息
 * @param fmt 格式
 * @param args 可变参数
 */
inline void DLOG_INFO(const LocationInfo& locationInfo, const std::string& fmt, Args... args) {
    constexpr size_t old_len = 128;
    char old_buffer[old_len];
    size_t new_len = snprintf(old_buffer, old_len, fmt.c_str(), args...);
    // 算上终止符'\0'
    new_len++;

    if (new_len > old_len) {
        std::vector<char> new_buffer(new_len);
        snprintf(new_buffer.data(), new_len, fmt.c_str(), args...);
        return LoggerFactory::instance()->getFirstLogger()->info(new_buffer.data(), locationInfo);
    }

    return LoggerFactory::instance()->getFirstLogger()->info(old_buffer, locationInfo);
}

template<typename... Args>
/**
 * @brief DLOG_INFO 打印info-log
 *
 * @param fmt 格式
 * @param args 可变参数
 */
inline void DLOG_INFO(const std::string& fmt, Args... args) {
    constexpr size_t old_len = 128;
    char old_buffer[old_len];
    size_t new_len = snprintf(old_buffer, old_len, fmt.c_str(), args...);
    // 算上终止符'\0'
    new_len++;

    if (new_len > old_len) {
        std::vector<char> new_buffer(new_len);
        snprintf(new_buffer.data(), new_len, fmt.c_str(), args...);
        return LoggerFactory::instance()->getFirstLogger()->info(new_buffer.data());
    }

    return LoggerFactory::instance()->getFirstLogger()->info(old_buffer);
}

template<typename... Args>
/**
 * @brief DLOG_DEBUG 打印debug-log
 *
 * @param locationInfo 位置信息
 * @param fmt 格式
 * @param args 可变参数
 */
inline void DLOG_DEBUG(const LocationInfo& locationInfo, const std::string& fmt, Args... args) {
    constexpr size_t old_len = 128;
    char old_buffer[old_len];
    size_t new_len = snprintf(old_buffer, old_len, fmt.c_str(), args...);
    // 算上终止符'\0'
    new_len++;

    if (new_len > old_len) {
        std::vector<char> new_buffer(new_len);
        snprintf(new_buffer.data(), new_len, fmt.c_str(), args...);
        return LoggerFactory::instance()->getFirstLogger()->debug(new_buffer.data(), locationInfo);
    }

    return LoggerFactory::instance()->getFirstLogger()->debug(old_buffer, locationInfo);
}

template<typename... Args>
/**
 * @brief DLOG_DEBUG 打印debug-log
 *
 * @param fmt 格式
 * @param args 可变参数
 */
inline void DLOG_DEBUG(const std::string& fmt, Args... args) {
    constexpr size_t old_len = 128;
    char old_buffer[old_len];
    size_t new_len = snprintf(old_buffer, old_len, fmt.c_str(), args...);
    // 算上终止符'\0'
    new_len++;

    if (new_len > old_len) {
        std::vector<char> new_buffer(new_len);
        snprintf(new_buffer.data(), new_len, fmt.c_str(), args...);
        return LoggerFactory::instance()->getFirstLogger()->debug(new_buffer.data());
    }

    return LoggerFactory::instance()->getFirstLogger()->debug(old_buffer);
}

template<typename... Args>
/**
 * @brief DLOG_ERROR 打印error-log
 *
 * @param locationInfo 位置信息
 * @param fmt 格式
 * @param args 可变参数
 */
inline void DLOG_ERROR(const LocationInfo& locationInfo, const std::string& fmt, Args... args) {
    constexpr size_t old_len = 128;
    char old_buffer[old_len];
    size_t new_len = snprintf(old_buffer, old_len, fmt.c_str(), args...);
    // 算上终止符'\0'
    new_len++;

    if (new_len > old_len) {
        std::vector<char> new_buffer(new_len);
        snprintf(new_buffer.data(), new_len, fmt.c_str(), args...);
        return LoggerFactory::instance()->getFirstLogger()->error(new_buffer.data(), locationInfo);
    }

    return LoggerFactory::instance()->getFirstLogger()->error(old_buffer, locationInfo);
}

template<typename... Args>
/**
 * @brief DLOG_ERROR 打印error-log
 *
 * @param fmt 格式
 * @param args 可变参数
 */
inline void DLOG_ERROR(const std::string& fmt, Args... args) {
    constexpr size_t old_len = 128;
    char old_buffer[old_len];
    size_t new_len = snprintf(old_buffer, old_len, fmt.c_str(), args...);
    // 算上终止符'\0'
    new_len++;

    if (new_len > old_len) {
        std::vector<char> new_buffer(new_len);
        snprintf(new_buffer.data(), new_len, fmt.c_str(), args...);
        return LoggerFactory::instance()->getFirstLogger()->error(new_buffer.data());
    }

    return LoggerFactory::instance()->getFirstLogger()->error(old_buffer);
}

template<typename... Args>
/**
 * @brief DLOG_FATAL 打印fatal-log
 *
 * @param locationInfo 位置信息
 * @param fmt 格式
 * @param args 可变参数
 */
inline void DLOG_FATAL(const LocationInfo& locationInfo, const std::string& fmt, Args... args) {
    constexpr size_t old_len = 128;
    char old_buffer[old_len];
    size_t new_len = snprintf(old_buffer, old_len, fmt.c_str(), args...);
    // 算上终止符'\0'
    new_len++;

    if (new_len > old_len) {
        std::vector<char> new_buffer(new_len);
        snprintf(new_buffer.data(), new_len, fmt.c_str(), args...);
        return LoggerFactory::instance()->getFirstLogger()->fatal(new_buffer.data(), locationInfo);
    }

    return LoggerFactory::instance()->getFirstLogger()->fatal(old_buffer, locationInfo);
}

template<typename... Args>
/**
 * @brief DLOG_FATAL 打印fatal-log
 *
 * @param fmt 格式
 * @param args 可变参数
 */
inline void DLOG_FATAL(const std::string& fmt, Args... args) {
    constexpr size_t old_len = 128;
    char old_buffer[old_len];
    size_t new_len = snprintf(old_buffer, old_len, fmt.c_str(), args...);
    // 算上终止符'\0'
    new_len++;

    if (new_len > old_len) {
        std::vector<char> new_buffer(new_len);
        snprintf(new_buffer.data(), new_len, fmt.c_str(), args...);
        return LoggerFactory::instance()->getFirstLogger()->fatal(new_buffer.data());
    }

    return LoggerFactory::instance()->getFirstLogger()->fatal(old_buffer);
}

//AsLoggerFactory
template<typename... Args>
/**
 * @brief DASLOG_TRACE 异步打印trace-log
 *
 * @param locationInfo 位置信息
 * @param fmt 格式
 * @param args 可变参数
 */
inline void DASLOG_TRACE(const LocationInfo& locationInfo, const std::string& fmt, Args... args) {
    constexpr size_t old_len = 128;
    char old_buffer[old_len];
    size_t new_len = snprintf(old_buffer, old_len, fmt.c_str(), args...);
    // 算上终止符'\0'
    new_len++;

    if (new_len > old_len) {
        std::vector<char> new_buffer(new_len);
        snprintf(new_buffer.data(), new_len, fmt.c_str(), args...);
        return AsLoggerFactory::instance()->getFirstLogger()->trace(new_buffer.data(), locationInfo);
    }

    return AsLoggerFactory::instance()->getFirstLogger()->trace(old_buffer, locationInfo);
}

template<typename... Args>
/**
 * @brief DASLOG_TRACE 异步打印trace-log
 *
 * @param fmt 格式
 * @param args 可变参数
 */
inline void DASLOG_TRACE(const std::string& fmt, Args... args) {
    constexpr size_t old_len = 128;
    char old_buffer[old_len];
    size_t new_len = snprintf(old_buffer, old_len, fmt.c_str(), args...);
    // 算上终止符'\0'
    new_len++;

    if (new_len > old_len) {
        std::vector<char> new_buffer(new_len);
        snprintf(new_buffer.data(), new_len, fmt.c_str(), args...);
        return AsLoggerFactory::instance()->getFirstLogger()->trace(new_buffer.data());
    }

    return AsLoggerFactory::instance()->getFirstLogger()->trace(old_buffer);
}

template<typename... Args>
/**
 * @brief DASLOG_INFO 异步打印info-log
 *
 * @param locationInfo 位置信息
 * @param fmt 格式
 * @param args 可变参数
 */
inline void DASLOG_INFO(const LocationInfo& locationInfo, const std::string& fmt, Args... args) {
    constexpr size_t old_len = 128;
    char old_buffer[old_len];
    size_t new_len = snprintf(old_buffer, old_len, fmt.c_str(), args...);
    // 算上终止符'\0'
    new_len++;

    if (new_len > old_len) {
        std::vector<char> new_buffer(new_len);
        snprintf(new_buffer.data(), new_len, fmt.c_str(), args...);
        return AsLoggerFactory::instance()->getFirstLogger()->info(new_buffer.data(), locationInfo);
    }

    return AsLoggerFactory::instance()->getFirstLogger()->info(old_buffer, locationInfo);
}

template<typename... Args>
/**
 * @brief DASLOG_INFO 异步打印info-log
 *
 * @param fmt 格式
 * @param args 可变参数
 */
inline void DASLOG_INFO(const std::string& fmt, Args... args) {
    constexpr size_t old_len = 128;
    char old_buffer[old_len];
    size_t new_len = snprintf(old_buffer, old_len, fmt.c_str(), args...);
    // 算上终止符'\0'
    new_len++;

    if (new_len > old_len) {
        std::vector<char> new_buffer(new_len);
        snprintf(new_buffer.data(), new_len, fmt.c_str(), args...);
        return AsLoggerFactory::instance()->getFirstLogger()->info(new_buffer.data());
    }

    return AsLoggerFactory::instance()->getFirstLogger()->info(old_buffer);
}

template<typename... Args>
/**
 * @brief DASLOG_DEBUG 异步打印debug-log
 *
 * @param locationInfo 位置信息
 * @param fmt 格式
 * @param args 可变参数
 */
inline void DASLOG_DEBUG(const LocationInfo& locationInfo, const std::string& fmt, Args... args) {
    constexpr size_t old_len = 128;
    char old_buffer[old_len];
    size_t new_len = snprintf(old_buffer, old_len, fmt.c_str(), args...);
    // 算上终止符'\0'
    new_len++;

    if (new_len > old_len) {
        std::vector<char> new_buffer(new_len);
        snprintf(new_buffer.data(), new_len, fmt.c_str(), args...);
        return AsLoggerFactory::instance()->getFirstLogger()->debug(new_buffer.data(), locationInfo);
    }

    return AsLoggerFactory::instance()->getFirstLogger()->debug(old_buffer, locationInfo);
}

template<typename... Args>
/**
 * @brief DASLOG_DEBUG 异步打印debug-log
 *
 * @param fmt 格式
 * @param args 可变参数
 */
inline void DASLOG_DEBUG(const std::string& fmt, Args... args) {
    constexpr size_t old_len = 128;
    char old_buffer[old_len];
    size_t new_len = snprintf(old_buffer, old_len, fmt.c_str(), args...);
    // 算上终止符'\0'
    new_len++;

    if (new_len > old_len) {
        std::vector<char> new_buffer(new_len);
        snprintf(new_buffer.data(), new_len, fmt.c_str(), args...);
        return AsLoggerFactory::instance()->getFirstLogger()->debug(new_buffer.data());
    }

    return AsLoggerFactory::instance()->getFirstLogger()->debug(old_buffer);
}

template<typename... Args>
/**
 * @brief DASLOG_ERROR 异步打印error-log
 *
 * @param locationInfo 位置信息
 * @param fmt 格式
 * @param args 可变参数
 */
inline void DASLOG_ERROR(const LocationInfo& locationInfo, const std::string& fmt, Args... args) {
    constexpr size_t old_len = 128;
    char old_buffer[old_len];
    size_t new_len = snprintf(old_buffer, old_len, fmt.c_str(), args...);
    // 算上终止符'\0'
    new_len++;

    if (new_len > old_len) {
        std::vector<char> new_buffer(new_len);
        snprintf(new_buffer.data(), new_len, fmt.c_str(), args...);
        return AsLoggerFactory::instance()->getFirstLogger()->error(new_buffer.data(), locationInfo);
    }

    return AsLoggerFactory::instance()->getFirstLogger()->error(old_buffer, locationInfo);
}

template<typename... Args>
/**
 * @brief DASLOG_ERROR 异步打印error-log
 *
 * @param fmt 格式
 * @param args 可变参数
 */
inline void DASLOG_ERROR(const std::string& fmt, Args... args) {
    constexpr size_t old_len = 128;
    char old_buffer[old_len];
    size_t new_len = snprintf(old_buffer, old_len, fmt.c_str(), args...);
    // 算上终止符'\0'
    new_len++;

    if (new_len > old_len) {
        std::vector<char> new_buffer(new_len);
        snprintf(new_buffer.data(), new_len, fmt.c_str(), args...);
        return AsLoggerFactory::instance()->getFirstLogger()->error(new_buffer.data());
    }

    return AsLoggerFactory::instance()->getFirstLogger()->error(old_buffer);
}

template<typename... Args>
/**
 * @brief DASLOG_FATAL 异步打印fatal-log
 *
 * @param locationInfo 位置信息
 * @param fmt 格式
 * @param args 可变参数
 */
inline void DASLOG_FATAL(const LocationInfo& locationInfo, const std::string& fmt, Args... args) {
    constexpr size_t old_len = 128;
    char old_buffer[old_len];
    size_t new_len = snprintf(old_buffer, old_len, fmt.c_str(), args...);
    // 算上终止符'\0'
    new_len++;

    if (new_len > old_len) {
        std::vector<char> new_buffer(new_len);
        snprintf(new_buffer.data(), new_len, fmt.c_str(), args...);
        return AsLoggerFactory::instance()->getFirstLogger()->fatal(new_buffer.data(), locationInfo);
    }

    return AsLoggerFactory::instance()->getFirstLogger()->fatal(old_buffer, locationInfo);
}

template<typename... Args>
/**
 * @brief DASLOG_FATAL 异步打印fatal-log
 *
 * @param fmt 格式
 * @param args 可变参数
 */
inline void DASLOG_FATAL(const std::string& fmt, Args... args) {
    constexpr size_t old_len = 128;
    char old_buffer[old_len];
    size_t new_len = snprintf(old_buffer, old_len, fmt.c_str(), args...);
    // 算上终止符'\0'
    new_len++;

    if (new_len > old_len) {
        std::vector<char> new_buffer(new_len);
        snprintf(new_buffer.data(), new_len, fmt.c_str(), args...);
        return AsLoggerFactory::instance()->getFirstLogger()->fatal(new_buffer.data());
    }

    return AsLoggerFactory::instance()->getFirstLogger()->fatal(old_buffer);
}

}
#endif /* __LOGGERFACTORY_HPP_ */
