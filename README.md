# DAQ__LOG

## 特性

1. 支持Json配置
2. 支持异步
3. 支持将日志发送HTTP、ZMQ
4. 支持滚动文件日志
5. 风格接近log4j，借鉴了sylar(https://github.com/sylar-yin/sylar)

## 使用建议

1. 配合Flume搭建日志服务器
2. HTTP、ZMQ可以使用异步log，其他情况不建议使用异步

## 依赖

项目依赖很多，但多数为常用库

1. czmq
2. concurrentqueue
3. boost.fiber
4. jsoncpp
5. curl

## 不提供TCP、UDP和syslog的Appender

本库的设计思想是配合Flume，搭建日志服务器；或者少量的本地调试

## License

Mozilla
