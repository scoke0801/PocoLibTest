//
// Created by gusl0 on 2021-08-04.
//

#include "LoggerTest.h"
#include "Poco/File.h"

#include "Poco/Path.h"
#include <iostream>
#include <asm/fcntl.h>

constexpr int MIN_PRIORITY = 1;
constexpr int MAX_PRIORITY = 8;

constexpr int CHANNEL_PROPERTY_NUM = 9;

typedef Poco::Channel*	(*ChannelCreatorFunc)();

ChannelCreatorFunc g_ChannelCreator[] = {
        []() ->Poco::Channel* { return nullptr; },
        []() ->Poco::Channel* { return new Poco::ConsoleChannel; },
        []() ->Poco::Channel* { return new Poco::ColorConsoleChannel; },
        []() ->Poco::Channel* { return new Poco::SimpleFileChannel; },
        []() ->Poco::Channel* { return new Poco::FileChannel; },
        []() ->Poco::Channel* { return new Poco::NullChannel; },
        []() ->Poco::Channel* { return new Poco::SplitterChannel; },
        []() ->Poco::Channel* { return new Poco::FormattingChannel; }
};
std::string g_ChannelProperties[] = {
        "path",
        "secondaryPath",
        "rotation",
        "archive",
        "times",
        "compress",
        "purgeAge",
        "purgeCount"
};
std::string g_ValidLogLevels[] = {
        "none",		  //(turns off logging)
        "fatal",	  // default priority of message :: PRIO_FATAL
        "critical",
        "error",
        "warning",
        "notice",
        "information", // default level of logger
        "debug",
        "trace",
};

//std::string g_LogDirPath = "/data/user/0/com.seerlsab.pocoMobileSample/files";
std::string g_LogDirPath = "storage/emulated/0/Android/data/com.seerlsab.pocoMobileSample/cache";
LogHandler::LogHandler()
{
    CreateDirectoryForLog(g_LogDirPath);
    BuildBasicLogger();
}

Poco::Logger* LogHandler::GetLogger(const std::string& loggerName)
{
    if (false == IsValidLoggerName(loggerName)) {
        // 처음 참조하는 경우
        m_Loggers.emplace(loggerName, &Poco::Logger::get(loggerName));
    }
    return m_Loggers[loggerName];
}

bool LogHandler::SetChannel(const std::string& loggerName, Channel channelName, void* properties)
{
    if (false == IsValidLoggerName(loggerName)) {
        return false;
    }

    auto logger = m_Loggers[loggerName];
    Poco::Channel* pChannel = nullptr;

    SetChannelOption(&pChannel, channelName, properties);

    logger->setChannel(pChannel);
    return true;
}

bool LogHandler::AddChannel(const std::string& loggerName, Channel channelName, void* properties)
{
    if (false == IsValidLoggerName(loggerName)) {
        return false;
    }
    auto logger = m_Loggers[loggerName];
    Poco::AutoPtr<Poco::SplitterChannel> castedChannel = logger->getChannel().cast<Poco::SplitterChannel>();
    poco_assert(castedChannel.get());

    Poco::Channel* pChannel = nullptr;
    SetChannelOption(&pChannel, channelName, properties);
    castedChannel->addChannel(pChannel);

    return true;
}

bool LogHandler::SetFormatter(const std::string& loggerName, Channel channelName, const std::string& formatText, void* properties)
{
    if (false == IsValidLoggerName(loggerName)) {
        return false;
    }
    auto logger = m_Loggers[loggerName];
    Poco::AutoPtr<Poco::FormattingChannel> castedChannel
            = logger->getChannel().cast<Poco::FormattingChannel>();
    poco_assert(castedChannel.get());

    castedChannel->setFormatter(new Poco::PatternFormatter(formatText));

    Poco::Channel* pChannel = nullptr;
    SetChannelOption(&pChannel, channelName, properties);
    castedChannel->setChannel(pChannel);

    return true;
}

void LogHandler::CreateDirectoryForLog(const std::string& path)
{
    Poco::File logDir(path);
    logDir.createDirectories();
}

void LogHandler::WriteLog(const std::string& loggerName, const std::string& text) {
    if (false == IsValidLoggerName(loggerName)) {
        return;
    }

    Poco::Message message;
    message.setText(text);
    m_Loggers[loggerName]->log(message);
}

void LogHandler::WriteLog(const std::string& loggerName, const std::string& text, const std::string& file, int codeLine)
{
    if (false == IsValidLoggerName(loggerName)) {
        return;
    }

    Poco::Message message;
    message.setSourceLine(codeLine);
    message.setSourceFile(file.c_str());
    message.setText(text);
    message.setTime(Poco::LocalDateTime().timestamp());
    m_Loggers[loggerName]->log(message);
}

void LogHandler::WriteLog(const std::string& loggerName, const Poco::Message& msg)
{
    if (false == IsValidLoggerName(loggerName)) {
        return;
    }
    auto isExist = m_Loggers[loggerName];
    m_Loggers[loggerName]->log(msg);
}

void LogHandler::SetLogLevel(const std::string& loggerName, const std::string& level)
{
    if (false == IsValidLoggerName(loggerName)) {
        return;
    }

    // 유효한 level이라면 설정하도록
    if (end(g_ValidLogLevels) != std::find(begin(g_ValidLogLevels), end(g_ValidLogLevels), level)) {
        m_Loggers[loggerName]->setLevel(level);
    }
}

void LogHandler::FreeAllLoggers()
{
    for (auto logger : m_Loggers) {
        // 하나의 로거를 대상으로 실행해도 전체 logger들이 초기화
        logger.second->shutdown();
        break;
    }
    m_Loggers.clear();
}

void LogHandler::FreeLogger(const std::string& loggerName)
{
    auto targetIter = m_Loggers.find(loggerName);
    if (targetIter == m_Loggers.end()) {
        return;
    }

    targetIter->second->release();
    m_Loggers.erase(targetIter);
}

bool LogHandler::IsSameLevel(const std::string& loggerName, const std::string& level)
{
    if (false == IsValidLoggerName(loggerName)) {
        return false;
    }

    int nLevel = -1;
    for (int i = 0; i < CHANNEL_PROPERTY_NUM; ++i) {
        if (level.compare(g_ValidLogLevels[i]) == 0) {
            nLevel = i;
            break;
        }
    }
    return m_Loggers[loggerName]->getLevel() == nLevel;
}

void LogHandler::SetChannelOption(Poco::Channel** channel, Channel channelName, void* properties)
{
    *channel = g_ChannelCreator[(int)channelName]();

    if (properties == nullptr) {
        return;
    }

    ChannelProperties* info = reinterpret_cast<ChannelProperties*>(properties);
    for (int i = 0; i < CHANNEL_PROPERTY_NUM; ++i) {
        if (0 != strlen(info->array[i])) {
            (*channel)->setProperty(g_ChannelProperties[i], info->array[i]);
        }
    }
}

void LogHandler::BuildBasicLogger()
{
    ChannelProperties property;
    std::string fileName;

    {
        GetLogger("Console");
        SetChannel("Console", Channel::Formatter, nullptr);
        SetFormatter("Console", Channel::Console, "[%Y-%m-%d  %H:%M:%s] [%U(%u)] %p: %t", nullptr);
    }

    {
        GetLogger("ColorConsole");
        SetChannel("ColorConsole", Channel::Formatter, nullptr);
        SetFormatter("ColorConsole", Channel::ColorConsole, "[%Y-%m-%d  %H:%M:%s] [%U(%u)] %p: %t", nullptr);
    }

    {
        property = {0};
        fileName = "SimpleFile";
        strcpy(property.path, Poco::format("%s/%s.log", g_LogDirPath, fileName).c_str());
        // 파일 크기 초과 시, 작성할 파일 이름
        strcpy(property.secondaryPath, Poco::format("%s/%s2.log", g_LogDirPath, fileName).c_str());
        // 파일 최대 크기, 1 MB로 지정
        strcpy(property.rotation, "1 M");

        GetLogger("SimpleFile");
        SetChannel("SimpleFile", Channel::SimpleFile, &property);
    }

    {
        property = {0};
        fileName = "FileLog";
        strcpy(property.path, Poco::format("%s/%s.log", g_LogDirPath, fileName).c_str());

        GetLogger("File");
        SetChannel("File", Channel::File, &property);
    }

    {
        property = {0};
        fileName = "Splitter";
        strcpy(property.path, Poco::format("%s/%s.log", g_LogDirPath, fileName).c_str());

        GetLogger("Splitter");
        SetChannel("Splitter", Channel::Splitter, nullptr);
        AddChannel("Splitter", Channel::Console, nullptr);
        AddChannel("Splitter", Channel::File, &property);
    }

    {
        property = {0};
        fileName = "Formatting";
        strcpy(property.path, Poco::format("%s/%s.log", g_LogDirPath, fileName).c_str());
        strcpy(property.times, "utc");
        strcpy(property.archive, "timestamp");

        GetLogger("Formatting");
        SetChannel("Formatting", Channel::Formatter, nullptr);

        //	[년-월-일 시간:분:출처] [파일경로(코드라인)] 우선순위: 텍스트
        SetFormatter("Formatting", Channel::File, "[%Y-%m-%d  %H:%M:%s] [%U(%u)] %p: %t", &property);
    }
}
