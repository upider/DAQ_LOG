#ifndef __FORMATTER_HPP_
#define __FORMATTER_HPP_

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <json/json.h>
#include "loglevel.hpp"
#include "logevent.hpp"

namespace DAQ {

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

}
#endif /*__FORMATTER_HPP_*/
