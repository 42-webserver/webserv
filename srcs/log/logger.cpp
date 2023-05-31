#include "../../incs/log/logger.hpp"

Logger::Logger()
    {
        fileStream.open(DEFAULT_LOG_FILE_NAME.c_str(), std::ofstream::out | std::ofstream::app);
        if (!fileStream) {
            std::cerr << "Log file could not be opened" << std::endl;
            outputStream = &std::cout;
        } else {
            outputStream = &fileStream;
        }
        logToStdOut = false;
        #ifdef DEBUG_FLAG
            logToStdOut = true;
        #endif
    }

Logger& Logger::getInstance()
{
    static Logger instance;
    return instance;
}

Logger::~Logger()
{
    if (outputStream != &std::cout && fileStream.is_open()) {
        fileStream.close();
    }
}

std::string Logger::converformatMessage(const std::string& format, int count, va_list args) {
    std::string message = format;

    for(int i = 0; i < count; i++) {
        size_t pos = message.find("{}");
        if (pos != std::string::npos) {
            std::string content = va_arg(args, char*);
            message.replace(pos, 2, content);
        }
    }

    return message;
}



void Logger::info(const std::string& message)
{
    log(INFO, message);
}

void Logger::info(const std::string& format, int count, ...)
{
    va_list args;
    va_start(args, count);
    std::string message = converformatMessage(format, count, args);
    va_end(args);
    log(INFO, message);
}

void Logger::warning(const std::string& message)
{
    log(WARNING, message);
}

void Logger::warning(const std::string& format, int count, ...)
{
    va_list args;
    va_start(args, count);
    std::string message = converformatMessage(format, count, args);
    va_end(args);
    log(WARNING, message);
}

void Logger::error(const std::string& message)
{
    log(ERROR, message);
}

void Logger::error(const std::string& format, int count, ...)
{
    va_list args;
    va_start(args, count);
    std::string message = converformatMessage(format, count, args);
    va_end(args);
    log(ERROR, message);
}

void Logger::debug(const std::string& message)
{
    log(DEBUG, message);
}

void Logger::debug(const std::string& format, int count, ...)
{
    va_list args;
    va_start(args, count);
    std::string message = converformatMessage(format, count, args);
    va_end(args);
    log(DEBUG, message);
}


void Logger::log(LogLevel level, const std::string& message)
{
    formattedMessage = formatMessage(level, message);

    if (logToStdOut) {
        std::cout << getLogColor(level) << formattedMessage << "\033[0m";
    }

    if (outputStream && outputStream != &std::cout) {
        (*outputStream) << formattedMessage;
    }
}

std::string Logger::getLogColor(LogLevel level)
{
    switch (level) {
        case INFO:
            return "\033[1;37m";   // White
        case WARNING:
            return "\033[1;33m";   // Yellow
        case ERROR:
            return "\033[1;31m";   // Red
        case DEBUG:
            return "\033[1;32m";   // Green
        default:
            return "";
    }
}

std::string Logger::formatMessage(LogLevel level, const std::string& message)
{
    std::ostringstream formattedMessageStream;

    std::time_t currentTime = std::time(NULL);
    std::tm* localTime = std::localtime(&currentTime);

    formattedMessageStream << "[" << (localTime->tm_year + 1900) << "-"
                           << std::setfill('0') << std::setw(2) << (localTime->tm_mon + 1) << "-"
                           << std::setfill('0') << std::setw(2) << localTime->tm_mday << " "
                           << std::setfill('0') << std::setw(2) << localTime->tm_hour << ":"
                           << std::setfill('0') << std::setw(2) << localTime->tm_min << ":"
                           << std::setfill('0') << std::setw(2) << localTime->tm_sec << "] ";

    switch (level) {
        case INFO:
            formattedMessageStream << "[INFO]: ";
            break;
        case WARNING:
            formattedMessageStream << "[WARNING]: ";
            break;
        case ERROR:
            formattedMessageStream << "[ERROR]: ";
            break;
        case DEBUG:
            formattedMessageStream << "[DEBUG]: ";
            break;
    }

    formattedMessageStream << message << std::endl;

    return formattedMessageStream.str();
}