#ifndef __LOGCONFIG_HPP_
#define __LOGCONFIG_HPP_

#include <string>
#include <vector>
#include "loglevel.hpp"

typedef struct LogConfigStruct {
    public:
        LogConfigStruct() {}
        LogConfigStruct(std::string name, LogLevel level)
            : LoggerName(name), outPutLevel(level) {}
        LogConfigStruct(std::string name, LogLevel level, size_t size)
            : LoggerName(name), outPutLevel(level), bufferSize(size) {}

    public:
        void operator=(const LogConfigStruct& rth) {
            this->LoggerName = rth.LoggerName;
            this->rowFormatter = rth.rowFormatter;
            this->jsonFormatter = rth.jsonFormatter;
            this->appenders = rth.appenders;
            this->singleFileName = rth.singleFileName;
            this->rollFilePath = rth.rollFilePath;
            this->filePrefix = rth.filePrefix;
            this->fileSubfix = rth.fileSubfix;
            this->inetAddr = rth.inetAddr;
            this->outPutLevel = rth.outPutLevel;
            this->port = rth.port;
            this->rollFileSize = rth.rollFileSize;
            this->bufferSize = rth.bufferSize;
        }
    public:
        std::string LoggerName = "root";
        std::string rowFormatter = "[%d{%Y-%m-%d %H:%M:%S}] [%p] [%f:%l] [%N] [%C] [%M] [%t] %m%n";
        std::string jsonFormatter = "";
        std::vector<std::string> appenders = {};
        std::string singleFileName = "";
        std::string rollFilePath = "";
        std::string filePrefix = "";
        std::string fileSubfix = "";
        std::string inetAddr = "";
        LogLevel outPutLevel = LogLevel::TRACE;
        size_t bufferSize = 0;
        size_t port = 0;
        size_t rollFileSize = 0;
} log_config_t;


class LogConfigurator {
    private:
        std::string m_configFile;
    public:
        std::string getConfFile() const;
        std::vector<log_config_t> setJsonConf(const std::string& filename);
    private:
        static std::vector<log_config_t> ReadJsonFromFile(const std::string& filename);

    public:
        LogConfigurator();
        virtual ~LogConfigurator();
};

#endif /* __LOGCONFIG_HPP_ */
