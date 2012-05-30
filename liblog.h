#ifndef __LIBLOG_H__
#define __LIBLOG_H__

#ifdef CONFIG_VERBOSE

#ifdef __MINGW32__
#define LOG_LEVEL(logLevel)   {SetLogLevel(logLevel);}

#define LOG_PREFIX(prefix) {SetMsgPrefix(prefix);}
#define LOG_FILE_NAME(lfn) {SetLogFileName(lfn);}
#define LOG_ERROR(msg, ...) {LogMsg(MSG_LEVEL_ERROR,(char*)__FILE__,(char*)__FUNCTION__,__LINE__,msg,##__VA_ARGS__);}
#define LOG_WARN(msg, ...) {LogMsg(MSG_LEVEL_WARN,(char*)__FILE__,(char*)__FUNCTION__,__LINE__,msg,##__VA_ARGS__);}
#define LOG_INFO(msg, ...) {LogMsg(MSG_LEVEL_INFO,(char*)__FILE__,(char*)__FUNCTION__,__LINE__,msg,##__VA_ARGS__);}
#define LOG_DEBUG(msg, ...) {LogMsg(MSG_LEVEL_DEBUG,(char*)__FILE__,(char*)__FUNCTION__,__LINE__,msg,##__VA_ARGS__);}

#define LOG_LEVEL_ERROR {LOG_LEVEL(MSG_LEVEL_ERROR)}
#define LOG_LEVEL_WARN {LOG_LEVEL(MSG_LEVEL_WARN)}
#define LOG_LEVEL_INFO {LOG_LEVEL(MSG_LEVEL_INFO)}
#define LOG_LEVEL_DEBUG {LOG_LEVEL(MSG_LEVEL_DEBUG)}

#define LOG_OUTPUT_DEBUG SetLogOutput(LOG_OUTPUT_TYPE_DEBUG)

#define LOG_OUTPUT_CONSOLE SetLogOutput(LOG_OUTPUT_TYPE_CONSOLE)

#define LOG_OUTPUT_FILE SetLogOutput(LOG_OUTPUT_TYPE_FILE)

#define LOG_OUTPUT_CONSOLE_AND_FILE SetLogOutput(LOG_OUTPUT_TYPE_CONSOLE | LOG_OUTPUT_TYPE_FILE)

#else //#ifdef __MINGW32__

#define LOG_LEVEL(logLevel)   {SetLogLevel(logLevel);}

#define LOG_PREFIX(prefix) {SetMsgPrefix(prefix);}
#define LOG_FILE_NAME(lfn) {SetLogFileName(lfn);}
#define LOG_ERROR(msg, ...) {LogMsg(MSG_LEVEL_ERROR,__FILE__,__FUNCTION__,__LINE__,msg,__VA_ARGS__);}
#define LOG_WARN(msg, ...) {LogMsg(MSG_LEVEL_WARN,__FILE__,__FUNCTION__,__LINE__,msg,__VA_ARGS__);}
#define LOG_INFO(msg, ...) {LogMsg(MSG_LEVEL_INFO,__FILE__,__FUNCTION__,__LINE__,msg,__VA_ARGS__);}
#define LOG_DEBUG(msg, ...) {LogMsg(MSG_LEVEL_DEBUG,__FILE__,__FUNCTION__,__LINE__,msg,__VA_ARGS__);}

#define LOG_LEVEL_ERROR {LOG_LEVEL(MSG_LEVEL_ERROR)}
#define LOG_LEVEL_WARN {LOG_LEVEL(MSG_LEVEL_WARN)}
#define LOG_LEVEL_INFO {LOG_LEVEL(MSG_LEVEL_INFO)}
#define LOG_LEVEL_DEBUG {LOG_LEVEL(MSG_LEVEL_DEBUG)}

#define LOG_OUTPUT_DEBUG SetLogOutput(LOG_OUTPUT_TYPE_DEBUG)

#define LOG_OUTPUT_CONSOLE SetLogOutput(LOG_OUTPUT_TYPE_CONSOLE)

#define LOG_OUTPUT_FILE SetLogOutput(LOG_OUTPUT_TYPE_FILE)

#define LOG_OUTPUT_CONSOLE_AND_FILE SetLogOutput(LOG_OUTPUT_TYPE_CONSOLE | LOG_OUTPUT_TYPE_FILE)

#endif //#ifdef __MINGW32__


#else // #ifdef CONFIG_VERBOSE

#define LOG_LEVEL

#define LOG_PREFIX(prefix)
#define LOG_FILE_NAME(lfn)
#define LOG_ERROR(msg, ...)
#define LOG_WARN(msg, ...)
#define LOG_INFO(msg, ...)
#define LOG_DEBUG(msg, ...)

#define LOG_LEVEL_ERROR
#define LOG_LEVEL_WARN
#define LOG_LEVEL_INFO
#define LOG_LEVEL_DEBUG

#define LOG_OUTPUT_DEBUG
#define LOG_OUTPUT_CONSOLE
#define LOG_OUTPUT_FILE 
#define LOG_OUTPUT_CONSOLE_AND_FILE

#endif // CONFIG_VERBOSE

#define LOG_OUTPUT_TYPE_DEBUG    1
#define LOG_OUTPUT_TYPE_CONSOLE  2
#define LOG_OUTPUT_TYPE_FILE     4

#define MSG_LEVEL_NONE  0
#define MSG_LEVEL_ERROR 1
#define MSG_LEVEL_WARN  2
#define MSG_LEVEL_INFO  3
#define MSG_LEVEL_DEBUG 4
#define MSG_LEVEL_MAX   5

#define PREFIX_BUFFER_SIZE  64
#define LINEFUNC_BUFFER_SIZE  128
#define TIME_BUFFER_SIZE    128
#define LOG_MSG_BUFFER_SIZE 1024
#define LOG_FILE_BUFFER_SIZE MAX_PATH

#ifdef __cplusplus
extern "C" {
#endif
void LogMsg(int msgLevel,char* fileName, char* funcName, int lineNum, char* msg, ...);
void SetMsgPrefix(char *msgPrefix);
void SetLogLevel(int logLevel);
void SetLogOutput(int logOutput);
void SetLogFileName(char *lfn);
#ifdef __cplusplus
}
#endif

#endif // __LIBLOG_H__
