#ifndef __LOGGER_HPP_
#define __LOGGER_HPP_

#include <string>
#include <memory>
#include <mutex>
#include <map>
#include <list>

#include <concurrentqueue/concurrentqueue.h>

#include "loglevel.hpp"
#include "logevent.hpp"
#include "logconfig.hpp"
#include "appender.hpp"

namespace daq {

//日志器
class Logger : public std::enable_shared_from_this<Logger> {
    public:
        using sptr = std::shared_ptr<Logger>;
        virtual void log(LogLevel level, const std::string& msg);
        virtual void log(LogLevel level, const std::string& msg, const LocationInfo& location);
        virtual void trace(const std::string& msg, const LocationInfo& location);
        virtual void trace(const std::string& msg);
        virtual void warn(const std::string& msg, const LocationInfo& location);
        virtual void warn(const std::string& msg);
        virtual void debug(const std::string& msg, const LocationInfo& location);
        virtual void debug(const std::string& msg);
        virtual void info(const std::string& msg, const LocationInfo& location);
        virtual void info(const std::string& msg);
        virtual void error(const std::string& msg, const LocationInfo& location);
        virtual void error(const std::string& msg);
        virtual void fatal(const std::string& msg, const LocationInfo& location);
        virtual void fatal(const std::string& msg);

        /**
         * @brief addAppender 添加输出端
         *
         * @param appender 输出端
         */
        virtual void addAppender(Appender* appender);
        /**
         * @brief delAppender 删除输出端
         *
         * @param appender 输出端
         */
        virtual void delAppender(Appender* appender);
        virtual void clearAppender();

        /**
         * @brief setOutputLevel 设置日志输出等级
         *
         * @param level 日志等级
         */
        virtual void setOutputLevel(LogLevel level) {
            m_conf.outputLevel = level;
        }

        /**
         * @brief getOutputLevel 获取输出的日志等级
         *
         * @return 日志等级
         */
        virtual LogLevel getOutputLevel() const {
            return m_conf.outputLevel;
        }

        /**
         * @brief setName 设置logger name
         *
         * @param name 日志名称
         */
        virtual void setName(const std::string& name) {
            m_conf.loggerName = name;
        }

        /**
         * @brief getName 获取日志名称
         *
         * @return 日志名称
         */
        virtual const std::string getName() const {
            return m_conf.loggerName;
        }

        /**
         * @brief setJsonFormatter 设置日志的Json格式
         *
         * @param formatter 格式
         */
        virtual void setJsonFormatter(const std::string& formatter) {
            m_conf.jsonFormatter = formatter;
            m_jsonFormatter.reset(new Formatter(formatter));
        }

        /**
         * @brief setJsonFormatter 设置日志的Json格式
         *
         * @param formatter 格式
         */
        virtual void setJsonFormatter(const Formatter& formatter) {
            m_jsonFormatter = std::make_shared<Formatter>(formatter);
            m_conf.jsonFormatter = formatter.getPattern();
        }

        /**
         * @brief setFormatter 设置日志的raw格式
         *
         * @param formatter raw格式
         */
        virtual void setFormatter(const std::string& formatter) {
            m_conf.rawFormatter = formatter;
            m_formatter.reset(new Formatter(formatter));
        }

        /**
         * @brief setFormatter 设置日志的raw格式
         *
         * @param formatter raw格式
         */
        virtual void setFormatter(const Formatter& formatter) {
            m_formatter = std::make_shared<Formatter>(formatter);
            m_conf.rawFormatter = formatter.getPattern();
        }

        /**
         * @brief setConfig 用配置结构体配置logger
         *
         * @param conf log_config_t
         */
        virtual void setConfig(const log_config_t& conf) {
            m_conf = conf;
        }

        /**
         * @brief getAllAppenderName 得到所有appender名字
         *
         * @return std::list<std::string>
         */
        virtual std::list<std::string> getAllAppenderName() {
            std::list<std::string> list;
            for (auto e : m_appendersMap) {
                list.push_back(e.first);
            }
            return list;
        }

    public:
        Logger(const std::string& name, const LogLevel level, size_t size = 256)
            : m_conf(name, level, size) {

            if (m_conf.rawFormatter != "") {
                m_formatter.reset(new Formatter(m_conf.rawFormatter));
            } else {
                m_formatter.reset(new Formatter("[%d{%Y-%m-%d %H:%M:%S}] [%p] [%f:%l] [%N] [%C] [%M] [%t] %m%n"));
            }

            if (m_conf.jsonFormatter != "") {
                m_jsonFormatter.reset(new Formatter(m_conf.jsonFormatter));
            } else {
                m_jsonFormatter.reset(new Formatter("{\"headers\":\"%N\",\"body\":{\"time\":\"%d{%Y-%m-%d %H:%M:%S}\",\"level\":\"%p\",\"file_name\":\"%f\",\"line\":\"%l\",\"class\":\"%C\",\"method\":\"%M\",\"thread_id\":\"%t\",\"msg\":\"%m\"}}"));
            }

        }

        virtual ~Logger() {
            clearAppender();
        }

    protected:
        log_config_t m_conf;
        std::map<std::string, Appender*> m_appendersMap;
        Formatter::sptr m_formatter;
        Formatter::sptr m_jsonFormatter;
        std::mutex m_mutex;
};

//AsLogger
class AsLogger: public Logger {
    public:
        using sptr = std::shared_ptr<AsLogger>;
        AsLogger(const std::string& name = "root",
                 const LogLevel level = LogLevel::TRACE, size_t size = 256)
            : Logger(name, level, size) {

            if (m_conf.rawFormatter != "") {
                m_formatter.reset(new Formatter(m_conf.rawFormatter));
            } else {
                m_formatter.reset(new Formatter("[%d{%Y-%m-%d %H:%M:%S}] [%p] [%f:%l] [%N] [%C] [%M] [%t] %m%n"));
            }

            if (m_conf.jsonFormatter != "") {
                m_jsonFormatter.reset(new Formatter(m_conf.jsonFormatter));
            } else {
                m_jsonFormatter.reset(new Formatter("[{\"headers\":{\"app_id\":\"%N\"},\"body\":\"%d{%Y-%m-%d %H:%M:%S},%p,%f:%l,%C,%M,%t,%m\"}]"));
            }

            m_buffer = moodycamel::ConcurrentQueue<LogEvent::sptr>(m_conf.asyncBufferSize);
            pullEvent(m_buffer, m_appendersMap);
        }

    public:
        static void pullEvent(moodycamel::ConcurrentQueue<LogEvent::sptr>& buffer,
                              std::map<std::string, Appender*>& appenders);

        size_t getBufferSize() const {
            return m_conf.asyncBufferSize;
        }

        virtual void log(LogLevel level, const std::string& msg) override;
        virtual void log(LogLevel level, const std::string& msg, const LocationInfo& location) override;

    private:
        moodycamel::ConcurrentQueue<LogEvent::sptr> m_buffer;
};

}
#endif /* __LOGGER_HPP_ */
