#ifndef _LOGGER_H_
#define _LOGGER_H_
#include <Windows.h>
#include <stdio.h>
#include <string>

namespace LOGGER
{
	//日志级别的提示信息
	static const std::string strFatalPrefix = "Fatal\t";
	static const std::string strErrorPrefix = "Error\t";
	static const std::string strWarningPrefix = "Warning\t";
	static const std::string strInfoPrefix = "Info\t";

	//日志级别枚举
	typedef enum EnumLogLevel
	{
		LogLevel_Stop = 0,	//什么都不记录
		LogLevel_Fatal,		//只记录严重错误
		LogLevel_Error,		//记录严重错误，普通错误
		LogLevel_Warning,	//记录严重错误，普通错误，警告
		LogLevel_Info		//记录严重错误，普通错误，警告，提示信息(也就是全部记录)
	};

	class CLogger
	{
	public:
		//nLogLevel：日志记录的等级，可空
		//strLogPath：日志目录，可空
		//strLogName：日志名称，可空
		CLogger(EnumLogLevel nLogLevel = EnumLogLevel::LogLevel_Info, const std::string strLogPath = "", const std::string strLogName = "");
		//析构函数
		virtual ~CLogger();
	public:
		//写严重错误信息
		void TraceFatal(const char *lpcszFormat, ...);
		//写错误信息
		void TraceError(const char *lpcszFormat, ...);
		//写警告信息
		void TraceWarning(const char *lpcszFormat, ...);
		//写提示信息
		void TraceInfo(const char *lpcszFormat, ...);
		//改变写日志级别
		void ChangeLogLevel(EnumLogLevel nLevel);
		//获取程序运行路径
		static std::string GetAppPathA();
		//格式化字符串
		static std::string FormatString(const char *lpcszFormat, ...);
	private:
		//写文件操作
		void Trace(const std::string &strLog);
		//获取当前系统时间
		std::string GetTime();
		//文件全路径得到文件名
		const char *path_file(const char *path, char splitter);
	private:
		//写日志文件流
		FILE * m_pFileStream;
		//写日志级别
		EnumLogLevel m_nLogLevel;
		//日志目录
		std::string m_strLogPath;
		//日志的名称
		std::string m_strLogName;
		//日志文件全路径
		std::string m_strLogFilePath;
		//线程同步的临界区变量
		CRITICAL_SECTION m_cs;
	};
}

#endif