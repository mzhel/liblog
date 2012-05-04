#include "liblog.h"
#include <stdio.h>
#include <windows.h>
#include <strsafe.h>

static char* prefix = NULL;
static char prefixBuffer[PREFIX_BUFFER_SIZE];
static char logFileName[LOG_FILE_BUFFER_SIZE];
static char timeBuffer[TIME_BUFFER_SIZE];
static int  currentLogLevel = MSG_LEVEL_INFO;
static int  outputType = LOG_OUTPUT_TYPE_DEBUG;
static int fullFileName = 0;

void SetMsgPrefix(char *msgPrefix)
{
    size_t newPrefixLen = 0;

    if(msgPrefix){

        if (SUCCEEDED(StringCchLength(msgPrefix, STRSAFE_MAX_CCH, &newPrefixLen))){

            if(newPrefixLen <= PREFIX_BUFFER_SIZE){

                if(SUCCEEDED(StringCchCopy(prefixBuffer, newPrefixLen + 1, msgPrefix))){

                    prefix = prefixBuffer;

                } else {

                    prefix = NULL;

                }

            }

        }

    }
}

void SetLogFileName(char *lfn)
{
    size_t newLfnLen = 0;

    if(lfn){

        if (SUCCEEDED(StringCchLength(lfn, STRSAFE_MAX_CCH, &newLfnLen))){

            if(newLfnLen <= LOG_FILE_BUFFER_SIZE){

                StringCchCopy(logFileName, newLfnLen + 1, lfn);

            }

        }

    }
}

void SetLogLevel(int logLevel)
{
    if(logLevel < MSG_LEVEL_MAX){

        currentLogLevel = logLevel;

    }
}

void SetLogOutput(int logOutput)
{
    outputType = logOutput;
}

static
char* GetMsgLevelStr(int msgLevel)
{
    char *msgLevelStr = "UNKNOWN";

    switch(msgLevel){

        case MSG_LEVEL_ERROR:

            msgLevelStr = "ERROR";

            break;

        case MSG_LEVEL_WARN:

            msgLevelStr = "WARN";

            break;

        case MSG_LEVEL_INFO:

            msgLevelStr = "INFO";

            break;

        case MSG_LEVEL_DEBUG:

            msgLevelStr = "DEBUG";

            break;
    }

    return msgLevelStr;
}

BOOL LogToConsole(const char *text)
{
    DWORD ioBytes = 0;
    DWORD len = 0;
    BOOL writeResult = FALSE;

    do {
        if (!text) break;

        len = lstrlen(text);

        if (!len) break;

        writeResult = WriteConsole(
                                   GetStdHandle(STD_OUTPUT_HANDLE),
                                   text,
                                   len,
                                   &ioBytes,
                                   NULL
                                   );

    } while (0);

    return writeResult;
}

BOOL LogToFile(const char *text)
{
    DWORD ioBytes = 0;
    DWORD len = 0;
    BOOL writeResult = FALSE;
    HANDLE file = INVALID_HANDLE_VALUE;
    DWORD result = 0;

    do {
        if (!text) break;

        len = lstrlen(text);

        if (!len) break;

        file = CreateFile(
                           logFileName,
                           GENERIC_READ | GENERIC_WRITE,
                           FILE_SHARE_READ,
                           NULL,
                           OPEN_ALWAYS,
                           FILE_ATTRIBUTE_NORMAL,
                           NULL
                           );

        if (file == INVALID_HANDLE_VALUE) break;

        result = SetFilePointer(file, 0, NULL, FILE_END);

        writeResult = WriteFile(
                                file,
                                text,
                                len,
                                &ioBytes,
                                NULL
                                );

    } while (0);

    if (file != INVALID_HANDLE_VALUE) CloseHandle(file);

    return writeResult;
}

void OutputLogStr(const char *str)
{
    if (outputType & LOG_OUTPUT_TYPE_DEBUG){

        OutputDebugString(str);

    }

    if (outputType & LOG_OUTPUT_TYPE_CONSOLE){

        LogToConsole(str);

    }

    if (outputType & LOG_OUTPUT_TYPE_FILE){

        LogToFile(str);

    }
}

void LogMsg(int msgLevel,char* fileName, char* funcName, int lineNum, char* msg, ...)
{
    char    outBuffer[LOG_MSG_BUFFER_SIZE];
    char    lineFuncBuffer[LINEFUNC_BUFFER_SIZE];
    size_t  prefixLen = 0,lfBufferLen = 0, fileNameLen = 0;
    char*   p = fileName;

    va_list args;

    if(msgLevel <= currentLogLevel){

        va_start(args,msg);

        if (!fullFileName){

            if (SUCCEEDED(StringCchLength(fileName, MAX_PATH, &fileNameLen))){

                p+= fileNameLen;

                while (*p != '\\' && *p != '/') p--;

                p++;

            }

        }

        if (prefix){

            // get prefix length

            if(SUCCEEDED(StringCchLength(prefix, PREFIX_BUFFER_SIZE, &prefixLen))){

                // copy prefix to main buffer

                if(FAILED(StringCchCopy(outBuffer, LOG_MSG_BUFFER_SIZE, prefix))){

                    // if something wrong happened, we just nullify prefixLen

                    prefixLen = 0;

                }

            }

        }

        GetTimeFormat(LOCALE_SYSTEM_DEFAULT, 0, NULL, NULL, timeBuffer, sizeof(timeBuffer));

        if(SUCCEEDED(StringCchPrintf(lineFuncBuffer, LINEFUNC_BUFFER_SIZE, "%s %s %s: %s:%d ", timeBuffer, p, GetMsgLevelStr(msgLevel), funcName, lineNum))){

            if(SUCCEEDED(StringCchLength(lineFuncBuffer, LINEFUNC_BUFFER_SIZE, &lfBufferLen))){

                if(SUCCEEDED(StringCchCat(outBuffer, LOG_MSG_BUFFER_SIZE, lineFuncBuffer))){

                    prefixLen += lfBufferLen;

                }

            }

        }

        if(SUCCEEDED(StringCchVPrintf(&outBuffer[prefixLen], LOG_MSG_BUFFER_SIZE, msg, args))){

            if(SUCCEEDED(StringCchCat(outBuffer, LOG_MSG_BUFFER_SIZE, "\n"))){

                OutputLogStr(outBuffer);

            }

        }

        va_end(args);
    }
}
