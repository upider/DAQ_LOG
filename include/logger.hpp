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

        virtual void addAppender(Appender* appender);
        virtual void delAppender(Appender* appender);
        virtual void clearAppender();

        virtual void setOutputLevel(LogLevel level) {
            m_conf.outPutLevel = level;
        }

        virtual LogLevel getOutputLevel() const {
            return m_conf.outPutLevel;
        }

        virtual void setName(const std::string& name) {
            m_conf.LoggerName = name;
        }

        virtual const std::string getName() const {
            return m_conf.LoggerName;
        }

        virtual void setJsonFormatter(const std::string& formatter) {
            m_conf.jsonFormatter = formatter;
            m_jsonFormatter.reset(new Formatter(formatter));
        }

        virtual void setJsonFormatter(const Formatter& formatter) {
            m_jsonFormatter = std::make_shared<Formatter>(formatter);
            m_conf.jsonFormatter = formatter.getPattern();
        }

        virtual void setFormatter(const std::string& formatter) {
            m_conf.rowFormatter = formatter;
            m_formatter.reset(new Formatter(formatter));
        }

        virtual void setFormatter(const Formatter& formatter) {
            m_formatter = std::make_shared<Formatter>(formatter);
            m_conf.rowFormatter = formatter.getPattern();
        }

        virtual void setConfig(const log_config_t& conf) {
            m_conf = conf;
        }

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

            if (m_conf.rowFormatter != "") {
                m_formatter.reset(new Formatter(m_conf.rowFormatter));
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

            if (m_conf.rowFormatter != "") {
                m_formatter.reset(new Formatter(m_conf.rowFormatter));
            } else {
                m_formatter.reset(new Formatter("[%d{%Y-%m-%d %H:%M:%S}] [%p] [%f:%l] [%N] [%C] [%M] [%t] %m%n"));
            }

            if (m_conf.jsonFormatter != "") {
                m_jsonFormatter.reset(new Formatter(m_conf.jsonFormatter));
            } else {
                m_jsonFormatter.reset(new Formatter("[{\"headers\":{\"app_id\":\"%N\"},\"body\":\"%d{%Y-%m-%d %H:%M:%S},%p,%f:%l,%C,%M,%t,%m\"}]"));
            }

            m_buffer = moodycamel::ConcurrentQueue<LogEvent::sptr>(m_conf.bufferSize);
            pullEvent(m_buffer, m_appendersMap);
        }

    public:
        static void pullEvent(moodycamel::ConcurrentQueue<LogEvent::sptr>& buffer,
                              std::map<std::string, Appender*>& appenders);

        size_t getBufferSize() const {
            return m_conf.bufferSize;
        }

        virtual void log(LogLevel level, const std::string& msg) override;
        virtual void log(LogLevel level, const std::string& msg, const LocationInfo& location) override;

    private:
        moodycamel::ConcurrentQueue<LogEvent::sptr> m_buffer;
};

#endif /* __LOGGER_HPP_ */
