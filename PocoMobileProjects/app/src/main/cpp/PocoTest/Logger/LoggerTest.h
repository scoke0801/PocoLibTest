//
// Created by gusl0 on 2021-08-04.
//

#ifndef MY_APPLICATION_LOGGERTEST_H
#define MY_APPLICATION_LOGGERTEST_H

#include <jni.h>
#include <string>
#include <unordered_map>

#include "Poco/Logger.h"
#include "Poco/LogStream.h"
#include "Poco/Message.h"
#include "Poco/AutoPtr.h"
#include "Poco/ConsoleChannel.h"
#include "Poco/SimpleFileChannel.h"
#include "Poco/FileChannel.h"
#include "Poco/File.h"
#include "Poco/NullChannel.h"
#include "Poco/SplitterChannel.h"
#include "Poco/FormattingChannel.h"
#include "Poco/PatternFormatter.h"
#include "Poco/DateTime.h"
#include "Poco/LocalDateTime.h"


enum class Channel : int {
    None,
    Console,
    ColorConsole,
    SimpleFile,
    File,
    Null,
    Splitter,
    Formatter
    // EventLog,	// only for Windows
    // Syslog		// only for Linux
};

constexpr int BUFFER_SIZE = 255;

union ChannelProperties {
    // 보기 쉽게 작성하려면 array로 접근
    char array[9][BUFFER_SIZE];

    // 명시적으로 작성하려면 아래 변수들로 접근
    struct {
        char path[BUFFER_SIZE];
        char secondaryPath[BUFFER_SIZE];
        char rotation[BUFFER_SIZE];
        char archive[BUFFER_SIZE];
        char times[BUFFER_SIZE];
        char compress[BUFFER_SIZE];
        char purgeAge[BUFFER_SIZE];
        char purgeCount[BUFFER_SIZE];
        char pattern[BUFFER_SIZE];
    };
};

class LogHandler {
public:
    static LogHandler &GetInstance() {
        static LogHandler self;
        return self;
    }

public:  // 주된 역할을 하는 함수들
    // 생성 or 가져오기
    Poco::Logger *GetLogger(const std::string &loggerName);

    // 로그 작성
    void WriteLog(const std::string &loggerName, const std::string &text);

    void
    WriteLog(const std::string &loggerName, const std::string &text, const std::string &file,
             int codeLine);

    void WriteLog(const std::string &loggerName, const Poco::Message &msg);

public: // 옵션 설정 함수들
    void SetLogLevel(const std::string &loggerName, const std::string &level);

    // 지정된 level로 설정 되어있는지 확인하는 함수
    bool IsSameLevel(const std::string &loggerName, const std::string &level);

    // 모든 Loggerr 객체 삭제
    void FreeAllLoggers();

    // 특정 Logger 객체 삭제
    void FreeLogger(const std::string &loggerName);

private: // 클래스 내부적으로 사용하는 함수들
    void SetChannelOption(Poco::Channel **channel, Channel ChannelInfo, void *properties);

    bool IsValidLoggerName(const std::string &loggerName) {
        return m_Loggers.find(loggerName) != m_Loggers.end();
    }

    // 채널 설정
    bool SetChannel(const std::string &loggerName, Channel channelName, void *properties);

    // SplitterChannel을 사용하는 경우
    bool AddChannel(const std::string &loggerName, Channel channelName, void *properties);

    // FormattingChannel을 사용하는 경우
    bool SetFormatter(const std::string &loggerName, Channel channelName,
                      const std::string &formatText, void *properties);

    // 지정된 경로에 맞게 폴더 생성
    void CreateDirectoryForLog(const std::string &path);

    // 사용할 Logger 객체들을 이 함수에서 미리 생성 및 설정합니다.
    void BuildBasicLogger();

private:
    LogHandler();

    ~LogHandler() {}

    // 복사생성 금지
    LogHandler(const LogHandler &other) = delete;

    // 대입연산 금지
    LogHandler &operator=(const LogHandler &other) = delete;

private:
    std::unordered_map<std::string, Poco::Logger *> m_Loggers;
};
#define LOGGER LogHandler::GetInstance()

#endif //MY_APPLICATION_LOGGERTEST_H
