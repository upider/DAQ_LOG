#ifndef __FORMATTER_HPP_
#define __FORMATTER_HPP_

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <json/json.h>
#include "loglevel.hpp"
#include "logevent.hpp"

/*
 * 自定义样式
 * %d 输出日志时间点的日期或时间，默认格式为ISO8601，也可以在其后指定格式，
 *    比如：%d{yyy MMM dd HH:mm:ss , SSS}，%d{ABSOLUTE}，%d{DATE}
 * %f 文件名
 * %F 协程名
 * %l 输出语句所在的行数
 * %m 输出代码中指定的讯息，如log(message)中的message
 * %M 输出方法名
 * %p 输出日志级别，即DEBUG，INFO，WARN，ERROR，FATAL
 * %r 输出自应用启动到输出该log信息耗费的毫秒数elapse
 * %t 输出产生该日志事件的线程名
 * %n 换行
 * %T TAB
 * %% 用来输出百分号“%”
 * %C 输出Logger所在类的名称，通常就是所在类的全名
 * %N logger name
 */

/**
 * @brief 日志格式化类
 */
class Formatter {
    public:
        using sptr = std::shared_ptr<Formatter>;
        void patternParser();
        virtual std::string format(LogEvent::sptr event);

    public:
        /**
         * @brief 构造函数
         *
         * @Param pattern 要使用的日志格式(字符串)
         */
        Formatter(const std::string& pattern);
        /**
         * @brief 构造函数
         */
        Formatter() = default;

        /**
         * @brief 析构函数
         */
        virtual ~Formatter() = default;

        std::string getPattern() const {
            return m_pattern;
        }

    public:
        //pattern解析
        class FormatItem {
            public:
                using sptr = std::shared_ptr<FormatItem>;
                FormatItem(const std::string& name): m_name(name) {}

                virtual std::string getName() const {
                    return m_name;
                }

                virtual ~FormatItem() = default;
                virtual void addFormat(std::ostream& os, LogEvent::sptr event) = 0;
            protected:
                std::string m_name = "";
        };

    protected:
        std::vector<FormatItem::sptr> m_items;
        std::string m_pattern;
};

//Formatter::FormatItem
/*******************************************************************************/
class LoggerNameFormatItem : public Formatter::FormatItem {
    public:
        LoggerNameFormatItem(const std::string& name = "app_id"): FormatItem("app_id") {}
        virtual void addFormat(std::ostream& os, LogEvent::sptr event) override {
            os << event->getLoggerName();
        }
};

class MessageFormatItem : public Formatter::FormatItem {
    public:
        MessageFormatItem(const std::string& name = "msg"): FormatItem("msg") {}
        virtual void addFormat(std::ostream& os, LogEvent::sptr event) override {
            os << event->getContent();
        }
};

class LevelFormatItem : public Formatter::FormatItem {
    public:
        LevelFormatItem(const std::string& name = "level"): FormatItem("level") {}
        virtual void addFormat(std::ostream& os, LogEvent::sptr event) override {
            os << LoglevelToStr(event->getLevel());
        }
};

class ThreadIdFormatItem : public Formatter::FormatItem {
    public:
        ThreadIdFormatItem(const std::string& name = "thread_id"): FormatItem("thread_id") {}
        virtual void addFormat(std::ostream& os, LogEvent::sptr event) override {
            os << event->getThreadId();
        }
};

class DateTimeFormatItem : public Formatter::FormatItem {
    public:
        DateTimeFormatItem(const std::string& str = "")
            : FormatItem("time"), m_fmt(str) {}
        virtual void addFormat(std::ostream& os, LogEvent::sptr event) override {
            time_t now = event->getTime();
            struct tm * time = localtime(&now);
            char buffer[128];
            std::strftime(buffer, 128, m_fmt.c_str(), time);
            os << buffer;
        }

    private:
        std::string m_fmt;
};

class FileNameFormatItem : public Formatter::FormatItem {
    public:
        FileNameFormatItem(const std::string& name = "file_name"): FormatItem("file_name") {}
        virtual void addFormat(std::ostream & os, LogEvent::sptr event) override {
            os << event->getFileName();
        }
};

class LineNumberFormatItem : public Formatter::FormatItem {
    public:
        LineNumberFormatItem(const std::string& name = "line"): FormatItem("line") {}
        virtual void addFormat(std::ostream& os, LogEvent::sptr event) override {
            os << event->getLineNumber();
        }
};

class FiberFormatItem : public Formatter::FormatItem {
    public:
        FiberFormatItem(const std::string& name = "fiber_id"): FormatItem("fiber_id") {}
        virtual void addFormat(std::ostream& os, LogEvent::sptr event) override {
            os << event->getFiberId();
        }
};

class ClassFormatItem : public Formatter::FormatItem {
    public:
        ClassFormatItem(const std::string& name = "class"): FormatItem("class") {}
        virtual void addFormat(std::ostream& os, LogEvent::sptr event) override {
            os << event->getClassName();
        }
};

class MethodFormatItem : public Formatter::FormatItem {
    public:
        MethodFormatItem(const std::string& name = "method"): FormatItem("method") {}
        virtual void addFormat(std::ostream& os, LogEvent::sptr event) override {
            os << event->getMethodName();
        }
};

class StrFormatItem : public Formatter::FormatItem {
    public:
        StrFormatItem(const std::string& str): FormatItem(""), m_str(str) {}
        virtual void addFormat(std::ostream & os, LogEvent::sptr event) override {
            os << m_str;
        }
    private:
        std::string m_str;
};

class TabFormatItem : public Formatter::FormatItem {
    public:
        TabFormatItem(const std::string& name = ""): FormatItem("") {}
        virtual void addFormat(std::ostream & os, LogEvent::sptr event) override {
            os << "\t";
        }
};

class NewLineFormatItem : public Formatter::FormatItem {
    public:
        NewLineFormatItem(const std::string& name = ""): FormatItem("") {}
        virtual void addFormat(std::ostream & os, LogEvent::sptr event) override {
            os << std::endl;
        }
};

#endif /*__FORMATTER_HPP_*/
