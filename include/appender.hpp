#ifndef __APPENDER_HPP_
#define __APPENDER_HPP_

#include <string>
#include <sstream>
#include <fstream>
#include <functional>
#include <mutex>

#include <czmq.h>
#include <curl/curl.h>

#include "loglevel.hpp"
#include "formatter.hpp"

/// \输出器的虚基类
class Appender {
    public:
        using sptr = std::shared_ptr<Appender>;
        /// \brief 默认构造函数
        Appender() = default;
        /// \brief 构造函数
        ///
        /// \param formater对象
        Appender(const Formatter formatter);
        /// \brief 构造函数
        ///
        /// \param formater字符串
        Appender(const std::string& formatter);
        /// \brief getId 得到Appender的id 避免相同LogAppender加入到Logger，使得重复输出
        ///
        /// \return id (string)
        const std::string getId() const {
            return m_id;
        }
        /// \brief 日志输出函数
        ///
        /// \param 日志事件
        virtual void append(LogEvent::sptr event) {};
        /// \brief 设置输出格式
        ///
        /// \param Formatter智能指针
        virtual void setFormatter(Formatter::sptr formatter);
        /// \brief 判断是否已经有了Formatter
        ///
        /// \return bool
        virtual bool hasFormatter();

    public:
        /// \brief 析构函数
        virtual ~Appender() = default;

    protected:
        std::string m_id = "Appender"; ///避免相同LogAppender加入到Logger，使得重复输出
        Formatter::sptr m_formatter;
        std::mutex m_appendMutex;
};

/// \brief StdoutAppender输出到控制台
class StdoutAppender : public Appender {
    public:
        /// \brief 构造函数
        StdoutAppender();
        virtual void append(LogEvent::sptr event) override;
        /// \brief 析构函数
        ~StdoutAppender() = default;
};

/// \FileRollAppender滚动输出到文件
class RollFileAppender : public Appender {
    public:
        /// \brief 默认构造函数
        RollFileAppender() = default;
        /// \brief 构造函数
        ///
        /// \param path 日志存放目录
        /// \param size 文件大小
        /// \param prefix 文件前缀
        /// \param subfix 文件后缀
        RollFileAppender(const std::string & path, u_int32_t size = 8,
                         const std::string & prefix = "", const std::string & subfix = "");
        /// \brief 析构函数
        ~RollFileAppender();

        /// \brief 日志输出函数
        ///
        /// \param 日志事件
        virtual void append(LogEvent::sptr event) override;

        void createNewFile();

        //重新打开返回true
        bool reopen();
        bool closeFile();

        /// \brief 设置输出目录
        ///
        /// \param 输出目录
        void setPath(const std::string & path) {
            m_path = path;
        }
        /// \brief 设置前缀
        ///
        /// \param 前缀
        void setPrefix(const std::string& prefix) {
            m_prefix = prefix;
        }
        /// \brief 设置后缀
        ///
        /// \param 后缀
        void setSubfix(const std::string& subfix) {
            m_subfix = subfix;
        }

    private:
        std::string m_path;
        size_t m_fileMaxSize;       ///文件最大大小MB
        std::string m_prefix;
        std::string m_subfix;
        std::string m_fileName;
        u_int32_t m_fileSize = 0;
        std::ofstream m_fileStream;
};

/// \FileAppender输出到指定文件
class SingleFileAppender : public Appender {
    public:
        SingleFileAppender();
        /// \brief 构造函数
        ///
        /// \param 日志文件名
        SingleFileAppender(const std::string& name);
        ~SingleFileAppender();

        virtual void append(LogEvent::sptr event) override;
        void setFileName(const std::string& filename) {
            m_fileName = filename;
        }
        bool reopen();

    private:
        std::string m_fileName;
        std::ofstream m_fileStream;
};

//ZMQ发送log,"inetAddr:port"
/*******************************************************************************/
class ZMQAppender : public Appender {
    public:
        ZMQAppender() {}
        ZMQAppender(const std::string& host, const std::string& port);
        ZMQAppender(const std::string& host, size_t port);
        ZMQAppender(const std::string& endpoint);
        ~ZMQAppender();

        virtual void append(LogEvent::sptr event) override;
        void setEndpoint(const std::string& endpoint) {
            m_endpoint = endpoint;
        }
        void setEndpoint(const std::string& host, size_t port) {
            m_host = host;
            m_port = std::to_string(port);
            m_endpoint = "tcp://" + m_host + ":" + m_port;
        }
        void setEndpoint(const std::string& host, const std::string& port) {
            m_host = host;
            m_port = port;
            m_endpoint = "tcp://" + m_host + ":" + m_port;
        }

    private:
        zsock_t *m_push;
        std::string m_host;
        std::string m_port;
        std::string m_endpoint;
        std::ofstream m_fileStream;
};

//HTTP发送JSON
/*******************************************************************************/
class HTTPAppender : public Appender {
    public:
        HTTPAppender(const std::string& host, const std::string& port);
        HTTPAppender(const std::string& host, size_t port);
        ~HTTPAppender();

        virtual void append(LogEvent::sptr event) override;

    private:
        int init();

    private:
        std::string m_host;
        long m_port;

        CURL *pCurl = NULL;
        std::string m_url;

        struct curl_slist *headerlist = NULL;
        struct curl_httppost *post = NULL;
        struct curl_httppost *last = NULL;
};

#endif /*__APPENDER_HPP_*/
