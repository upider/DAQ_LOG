#include <iostream>
#include <fstream>
#include <json/json.h>
#include "logconfig.hpp"

namespace DAQ {

LogConfigurator::LogConfigurator() {}

LogConfigurator::~LogConfigurator() {}

std::string LogConfigurator::getConfFile() const {
    return m_configFile;
}

std::vector<log_config_t> LogConfigurator::setJsonConf(const std::string& filename) {
    m_configFile = filename;
    return ReadJsonFromFile(filename);
}

//typedef struct LogConfigStruct {
//    std::string LoggerName;
//	  conf.rowFormatter;
//    conf.jsonFormatter;
//	  std::string[] appenders
//    std::string singleFileName;
//    std::string rollFilePath;
//    std::string filePrefix;
//    std::string fileSubfix;
//    std::string inetAddr;
//    LogLevel outPutLevel;
//    size_t port;
//    size_t rollFileSize;
//} log_config_t;


std::vector<log_config_t> LogConfigurator::ReadJsonFromFile(const std::string& filename) {
    Json::CharReaderBuilder readerBuilder;
    Json::Value value; // Json::Value是一种很重要的类型，可以代表任意类型。如int, string, object, array

    std::ifstream in(filename, std::ios::binary);
    std::vector<log_config_t> confs;
    if (!in.is_open()) {
        std::cout << "LogConfigurator::ReadJsonFromFile open file error" << std::endl;
        return confs;
    }
    JSONCPP_STRING errs;
    if(Json::parseFromStream(readerBuilder, in, &value, &errs)) {
        for (unsigned int i = 0; i < value["Loggers"].size(); ++i) {
            log_config_t conf;
            conf.LoggerName = value["Loggers"][i]["name"].asString();
            conf.rowFormatter = value["Loggers"][i]["rowFormatter"].asString();
            conf.jsonFormatter = value["Loggers"][i]["jsonFormatter"].asString();

            for (unsigned int j = 0; j < value["Loggers"][i]["appenders"].size(); ++j) {
                conf.appenders.emplace_back(value["Loggers"][i]["appenders"][j].asString());
            }

            conf.singleFileName = value["Loggers"][i]["singleFileName"].asString();
            conf.rollFilePath = value["Loggers"][i]["rollFilePath"].asString();
            conf.filePrefix = value["Loggers"][i]["filePrefix"].asString();
            conf.fileSubfix = value["Loggers"][i]["fileSubfix"].asString();
            conf.inetAddr = value["Loggers"][i]["inetAddr"].asString();
            conf.outPutLevel = LogLevel(value["Loggers"][i]["outPutLevel"].asInt());
            conf.port = value["Loggers"][i]["port"].asInt();
            conf.rollFileSize = value["Loggers"][i]["rollFileSize"].asInt();
            conf.bufferSize = value["Loggers"][i]["bufferSize"].asInt();
            confs.push_back(conf);
        }
        in.close();
        return confs;
    } else {
        std::cout << "ERROR: " << errs << std::endl;
        return confs;
    }
}

}
