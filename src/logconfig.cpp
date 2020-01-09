#include <iostream>
#include <fstream>
#include <stdexcept>

#include <boost/filesystem.hpp>
#include <tinyxml2.h>
#include <json/json.h>

#include "logconfig.hpp"

namespace DAQ {

std::string LogConfigurator::getConfFileName() const {
    return m_configFile;
}

std::vector<log_config_t> LogConfigurator::getConf(const std::string& filename) {
    m_configFile = filename;
    if(!boost::filesystem::exists(filename)) {
        throw std::runtime_error("file not exists!");
    }
    boost::filesystem::path p(filename);
    if (p.extension() == ".json") {
        return readJsonFromFile(filename);
    } else if(p.extension() == ".xml") {
        return readXMLFromFile(filename);
    }
    else {
        return readTxtFromFile(filename);
    }
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


std::vector<log_config_t> LogConfigurator::readJsonFromFile(const std::string& filename) {
    Json::CharReaderBuilder readerBuilder;
    Json::Value value;

    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open()) {
        throw std::runtime_error("LogConfigurator::readJsonFromFile open file error!");
    }

    JSONCPP_STRING errs;
    std::vector<log_config_t> confs;

    if(Json::parseFromStream(readerBuilder, in, &value, &errs)) {
        for (unsigned int i = 0; i < value["loggers"].size(); ++i) {
            log_config_t conf;
            conf.loggerName = value["loggers"][i]["name"].asString();
            conf.rawFormatter = value["loggers"][i]["rawFormatter"].asString();
            conf.jsonFormatter = value["loggers"][i]["jsonFormatter"].asString();

            for (unsigned int j = 0; j < value["loggers"][i]["appenders"].size(); ++j) {
                conf.appenders.emplace_back(value["loggers"][i]["appenders"][j].asString());
            }

            conf.singleFileName = value["loggers"][i]["singleFileName"].asString();
            conf.rollFilePath = value["loggers"][i]["rollFilePath"].asString();
            conf.rollFilePrefix = value["loggers"][i]["filePrefix"].asString();
            conf.rollFileSubfix = value["loggers"][i]["fileSubfix"].asString();
            conf.inetAddr = value["loggers"][i]["inetAddr"].asString();
            conf.outputLevel = LogLevel(value["loggers"][i]["outPutLevel"].asInt());
            conf.port = value["loggers"][i]["port"].asInt();
            conf.rollFileSize = value["loggers"][i]["rollFileSize"].asInt();
            conf.asyncBufferSize = value["loggers"][i]["bufferSize"].asInt();
            confs.push_back(conf);
        }
        in.close();
        return confs;
    } else {
        std::cout << "ERROR: " << errs << std::endl;
        return confs;
    }
}

std::vector<log_config_t> LogConfigurator::readXMLFromFile(const std::string& filename) {
    using namespace tinyxml2;
    XMLDocument doc;

    if(XML_SUCCESS != doc.LoadFile(filename.c_str())) {
        throw std::runtime_error("LogConfigurator::readXMLFromFile load file error!");
    }

    std::vector<log_config_t> confs;

    XMLElement *loggers = doc.RootElement();
    XMLElement* logger = loggers->FirstChildElement("logger");
    if(loggers) {
        while (logger) {
            log_config_t conf;
            auto ele = logger->FirstChildElement("name");
            if (ele) {
                conf.loggerName = ele->GetText();
            }
            ele = logger->FirstChildElement("rawFormatter");
            if (ele) {
                conf.rawFormatter = ele->GetText();
            }
            ele = logger->FirstChildElement("jsonFormatter");
            if (ele) {
                conf.jsonFormatter = ele->GetText();
            }
            ele = logger->FirstChildElement("singleFileName");
            if (ele) {
                conf.singleFileName = ele->GetText();
            }
            ele = logger->FirstChildElement("rollFilePath");
            if (ele) {
                conf.rollFilePath = ele->GetText();
            }
            ele = logger->FirstChildElement("rollFilePrefix");
            if (ele) {
                conf.rollFilePrefix = ele->GetText();
            }
            ele = logger->FirstChildElement("rollFileSubfix");
            if (ele) {
                conf.rollFileSubfix = ele->GetText();
            }
            ele = logger->FirstChildElement("inetAddr");
            if (ele) {
                conf.inetAddr = ele->GetText();
            }
            ele = logger->FirstChildElement("port");
            if (ele) {
                conf.port = std::stoul(ele->GetText());
            }
            ele = logger->FirstChildElement("outputLevel");
            if (ele) {
                conf.outputLevel = LogLevel(std::stoul(ele->GetText()));
            }
            ele = logger->FirstChildElement("asyncBufferSize");
            if (ele) {
                conf.asyncBufferSize = std::stol(ele->GetText());
            }

            ///获取Appenders,可能不止一个
            const XMLElement* appenders = logger->FirstChildElement("appenders");
            if (appenders) {
                const XMLElement *appender = appenders->FirstChildElement("appender");
                while(appender) {
                    conf.appenders.push_back(appender->GetText());
                    appender = appender->NextSiblingElement("appender");
                }
            }

            confs.push_back(conf);
            logger = logger->NextSiblingElement("logger");
        }
    } else {
        throw std::runtime_error("LogConfigurator::readXMLFromFile parser file error!");
    }
    return confs;
}

std::vector<log_config_t> LogConfigurator::readTxtFromFile(const std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open()) {

    }
}

}
