#include <iostream>
#include <thread>
#include <chrono>
#include <stdexcept>
#include <sys/stat.h>
#include <json/json.h>
#include "appender.hpp"

//Appender
Appender::Appender(const Formatter formater) {
    m_formatter = std::make_shared<Formatter>(formater);
}

Appender::Appender(const std::string& formater) {
    m_formatter.reset(new Formatter(formater));
}

void Appender::setFormatter(Formatter::sptr formatter) {
    m_formatter = formatter;
}

bool Appender::hasFormatter() {
    if(m_formatter)
        return true;
    return false;
}

//StdoutAppender
/*******************************************************************************/
StdoutAppender::StdoutAppender() {
    m_id += "::StdoutAppender";
}

void StdoutAppender::append(LogEvent::sptr event) {
    std::lock_guard<std::mutex> lock_guard(m_appendMutex);
    std::clog << m_formatter->format(event);
}

//Rolender
/*******************************************************************************/
RollFileAppender::RollFileAppender(const std::string & path, u_int32_t size,
                                   const std::string & prefix, const std::string & subfix)
    : m_path(path),
      m_fileMaxSize(size) {
    std::stringstream ss;
    ss << "::FileRollAppender::" << prefix << "-" << subfix;
    m_id += ss.str();
    if(m_fileMaxSize == 0) {
        std::cout << " file size is invaild!" << std::endl;
        return;
    }
    createNewFile();
    reopen();
}

RollFileAppender::~RollFileAppender() {
    std::cout << "~RollFileAppender()"  << std::endl;
    closeFile();
}

void RollFileAppender::createNewFile() {
    time_t tt = time(0);
    struct tm * time = gmtime(&tt);
    char buffer[128];
    std::strftime(buffer, 128, "%Y%m%d%H%M%S", time);
    m_fileName = m_prefix + buffer + m_subfix;
}

bool RollFileAppender::closeFile() {
    if (m_fileStream.is_open())
        m_fileStream.close();
    return !m_fileStream.is_open();
}

void RollFileAppender::append(LogEvent::sptr event) {
    struct stat statbuf;
    stat(m_fileName.c_str(), &statbuf);
    std::lock_guard<std::mutex> lock_guard(m_appendMutex);
    if ((double)statbuf.st_size / (1024 * 1024) <= 8) {
        m_fileStream << m_formatter->format(event);
        m_fileSize = statbuf.st_size;
    } else {
        closeFile();
        createNewFile();
        reopen();
        m_fileStream << m_formatter->format(event);
    }
}

bool RollFileAppender::reopen() {
    if (m_fileStream.is_open())
        m_fileStream.close();
    m_fileStream.open(m_fileName);

    return m_fileStream.is_open();
}

//SingleFileAppender不要多个Logger使用同一个文件
/*******************************************************************************/
SingleFileAppender::SingleFileAppender() {}

SingleFileAppender::SingleFileAppender(const std::string & name): m_fileName(name) {
    std::stringstream ss;
    ss << "::FileAppender:" << m_fileName;
    m_id += ss.str();
    if (!reopen()) {
        std::cout << "SingleFileAppender open file error!"  << std::endl;
    }
}

void SingleFileAppender::append(LogEvent::sptr event) {
    std::lock_guard<std::mutex> lock_guard(m_appendMutex);
    m_fileStream << m_formatter->format(event);
    m_fileStream.flush();
}

bool SingleFileAppender::reopen() {
    if (m_fileStream.is_open())
        m_fileStream.close();
    m_fileStream.open(m_fileName, std::ios::out);

    return m_fileStream.is_open();
}

SingleFileAppender::~SingleFileAppender() {
    if(m_fileStream.is_open())
        m_fileStream.close();
}

//ZMQAppender
/*******************************************************************************/
ZMQAppender::ZMQAppender(const std::string & endpoint)
    : m_endpoint(endpoint) {
    std::stringstream ss;
    ss << "::ZMQAppender:" + m_endpoint;
    m_id += ss.str();
    m_push = zsock_new_push(m_endpoint.c_str());
}

ZMQAppender::ZMQAppender(const std::string& host, const std::string& port)
    : m_host(host),
      m_port(port),
      m_endpoint("tcp://" + host + ":" + port) {}

ZMQAppender::ZMQAppender(const std::string& host, size_t port)
    : m_host(host),
      m_port(std::to_string(port)),
      m_endpoint("tcp://" + host + ":" + std::to_string(port)) {}

void ZMQAppender::append(LogEvent::sptr event) {
    std::lock_guard<std::mutex> lock_guard(m_appendMutex);
    auto str = m_formatter->format(event);
    zstr_send(m_push, m_formatter->format(event).c_str());
}

ZMQAppender::~ZMQAppender() {
    zsock_destroy(&m_push);
}

//HTTP发送JSON
/*******************************************************************************/
HTTPAppender::HTTPAppender(const std::string& host, const std::string& port)
    : m_host(host), m_port(std::stol(port)), m_url("http://" + host + ":" + port) {
    if (init() == -1) {
        return;
    }
}

HTTPAppender::HTTPAppender(const std::string& host, size_t port)
    : m_host(host), m_port(port), m_url("http://" + host + ":" + std::to_string(port)) {
    if (init() == -1) {
        return;
    }
}

void HTTPAppender::append(LogEvent::sptr event) {
    if (NULL == pCurl) {
        std::cout << __FILE__ << ":" << __LINE__ << " CURLE_FAILED_INIT" << std::endl;
        return ;
    }

    auto jsonOut = m_formatter->format(event);
    //std::cout << jsonOut << std::endl;
    // 设置要POST的JSON数据
    curl_easy_setopt(pCurl, CURLOPT_POSTFIELDS, jsonOut.c_str());
    curl_easy_setopt(pCurl, CURLOPT_POSTFIELDSIZE, jsonOut.size());//设置上传json串长度,这个设置可以忽略
    char errbuf[128];
    curl_easy_setopt(pCurl, CURLOPT_ERRORBUFFER, errbuf);
    CURLcode res = curl_easy_perform(pCurl);
    if(res != CURLE_OK) {
        size_t len = strlen(errbuf);
        fprintf(stderr, "\nlibcurl: (%d) ", res);
        if(len)
            fprintf(stderr, "%s%s", errbuf,
                    ((errbuf[len - 1] != '\n') ? "\n" : ""));
        else
            fprintf(stderr, "%s\n", curl_easy_strerror(res));
    }
}

int HTTPAppender::init() {
    std::stringstream ss;
    ss << "::HTTPAppender:" << m_host << ":" << m_port;
    m_id += ss.str();


    pCurl = curl_easy_init();
    if (NULL == pCurl)
    {
        std::cout << __FILE__ << ":" << __LINE__ << " CURLE_FAILED_INIT" << std::endl;
        return -1;
    }

    //设置http发送的内容类型为JSON
    //构建HTTP报文头
    headerlist = curl_slist_append(headerlist, "Content-Type:application/json;charset=UTF-8");

    curl_easy_setopt(pCurl, CURLOPT_URL, m_url.c_str());
    curl_easy_setopt(pCurl, CURLOPT_POST, 1);//设置为非0表示本次操作为POST
    curl_easy_setopt(pCurl, CURLOPT_HTTPHEADER, headerlist);

    return 0;
}

HTTPAppender::~HTTPAppender() {
    if (pCurl) {
        curl_easy_cleanup(pCurl);
    }
}
