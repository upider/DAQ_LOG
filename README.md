# DAQ__LOG

## 特性

1. 支持Json配置
2. 支持异步
3. 支持将日志发送HTTP、ZMQ
4. 支持滚动文件日志
5. 风格接近log4j，借鉴了sylar(https://github.com/sylar-yin/sylar)和log4j
6. 支持Linux平台(ubuntu 18.04)

## 使用建议

	1. 配合Flume搭建日志服务器
	2. HTTP、ZMQ可以使用异步log，其他情况不建议使用异步

## 依赖

	项目依赖很多，但多数为常用库

	1. czmq
	2. concurrentqueue
	3. boost.fiber filesystem
	4. jsoncpp
	5. curl
	6. tinyxml2

## 自定义日志样式
	比如：%d{yyy MMM dd HH:mm:ss , SSS}
	%f 文件名
	%F 协程名
	%l 输出语句所在的行数
	%m 输出代码中指定的讯息，如log(message)中的message
	%M 输出方法名
	%p 输出日志级别，即DEBUG，INFO，WARN，ERROR，FATAL
	%r 输出自应用启动到输出该log信息耗费的毫秒数elapse
	%t 输出产生该日志事件的线程名
	%n 换行
	%T TAB
	%% 用来输出百分号“%”
	%C 输出Logger所在类的名称，通常就是所在类的全名
	%N logger name

	例子1-简单样式：[%d{%Y-%m-%d %H:%M:%S}] [%p] [%f:%l] [%N] [%C] [%M] [%t] %m%n
	例子2-json日志样式：[{\"headers\":{\"app_id\":\"%N\"},\"body\":\"%d{%Y-%m-%d %H:%M:%S},%p,%f:%l,%C,%M,%t,%m\"}]

	json日志样式适合配合HTTPAppender发送给Flume

## Appender——日志输出器

	1. StdoutAppender：输出到标准输出
	2. ZMQAppender：zmq管道模式输出到服务器端
	3. SingleFileAppender：单文件日志输出
	4. RollFileAppender：滚动文件日志输出
	5. HTTPAppender：HTTP发送日志到服务器端,适合发送到Flume

## 不提供TCP、UDP和syslog的Appender

	本库的设计思想是配合Flume，搭建日志服务器；或者本地调试

## License

	Mozilla
