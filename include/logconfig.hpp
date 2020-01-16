#ifndef __LOGCONFIG_HPP_
#define __LOGCONFIG_HPP_

#include <string>
#include <vector>
#include "loglevel.hpp"

namespace daq {

/// @brief 配置log的结构体，包含所有能够配置的选项
typedef struct LogConfigStruct {
    public:
        LogConfigStruct(const LogConfigStruct&& rth);
        LogConfigStruct(const LogConfigStruct& rth);
        LogConfigStruct() = default;
        explicit LogConfigStruct(std::string name, LogLevel level)
            : loggerName(name),
              outputLevel(level) {}
        explicit LogConfigStruct(std::string name, LogLevel level, size_t size)
            : loggerName(name),
              asyncBufferSize(size),
              outputLevel(level) {}

    public:
        LogConfigStruct& operator=(const LogConfigStruct&& rth) {
            if (this == &rth) {
                return *this;
            }
            this->loggerName = rth.loggerName;
            this->rawFormatter = rth.rawFormatter;
            this->jsonFormatter = rth.jsonFormatter;
            this->appenders = rth.appenders;
            this->singleFileName = rth.singleFileName;
            this->rollFilePath = rth.rollFilePath;
            this->rollFilePrefix = rth.rollFilePrefix;
            this->rollFileSubfix = rth.rollFileSubfix;
            this->inetAddr = rth.inetAddr;
            this->port = rth.port;
            this->rollFileSize = rth.rollFileSize;
            this->asyncBufferSize  = rth.asyncBufferSize;
            this->outputLevel = rth.outputLevel;

            return *this;
        }

        LogConfigStruct& operator=(const LogConfigStruct& rth) {
            if (this == &rth) {
                return *this;
            }

            this->loggerName = rth.loggerName;
            this->rawFormatter = rth.rawFormatter;
            this->jsonFormatter = rth.jsonFormatter;
            this->appenders = rth.appenders;
            this->singleFileName = rth.singleFileName;
            this->rollFilePath = rth.rollFilePath;
            this->rollFilePrefix = rth.rollFilePrefix;
            this->rollFileSubfix = rth.rollFileSubfix;
            this->inetAddr = rth.inetAddr;
            this->port = rth.port;
            this->rollFileSize = rth.rollFileSize;
            this->asyncBufferSize  = rth.asyncBufferSize;
            this->outputLevel = rth.outputLevel;

            return *this;
        }

    public:
        std::string loggerName = "root";
        std::string rawFormatter = "[%d{%Y-%m-%d %H:%M:%S}] [%p] [%f:%l] [%N] [%C] [%M] [%t] %m%n";
        std::string jsonFormatter = "";
        std::vector<std::string> appenders = {};
        std::string singleFileName = "";
        std::string rollFilePath = "";
        std::string rollFilePrefix = "";
        std::string rollFileSubfix = "";
        size_t rollFileSize = 0;
        std::string inetAddr = "";
        size_t port = 0;
        size_t asyncBufferSize = 0;
        LogLevel outputLevel = LogLevel::TRACE;
} log_config_t;


/// @brief 能够配置log的类
class LogConfigurator {
    private:
        std::string m_configFile;
    public:
        /**
         * @brief getConfFileName 得到配置文件名
         *
         * @return 配置文件名
         */
        std::string getConfFileName() const;
        /**
         * @brief getConf 根据配置文件返回配置结构体
         *
         * @param filename 配置文件名
         *
         * @return 配置文件结构体数组
         */
        std::vector<log_config_t> getConf(const std::string& filename);
    private:
        static std::vector<log_config_t> readJsonFromFile(const std::string& filename);
        static std::vector<log_config_t> readXMLFromFile(const std::string& filename);

    public:
        LogConfigurator() = default;
        virtual ~LogConfigurator() = default;
};

}
#endif /* __LOGCONFIG_HPP_ */
