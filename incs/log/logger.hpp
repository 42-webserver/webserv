#ifndef LOGGER_HPP
# define LOGGER_HPP

# include <fstream>
# include <sstream>
# include <iostream>
# include <ctime>

const std::string DEFAULT_LOG_FILE_NAME = "../../LogSave/Log.txt";

enum LogLevel {
    INFO,
    WARNING,
    ERROR,
    DEBUG
};

class Logger {
private:
    std::ostream* outputStream;
    std::ofstream fileStream;
    bool logToStdOut;
    std::string formattedMessage;

    Logger();

public:
    static Logger& getInstance();

    ~Logger();
    void info(const std::string& message) ;

    void info(const std::string& format, const std::string& content); 

    void warning(const std::string& message) ;
    void warning(const std::string& format, const std::string& content); 
    void error(const std::string& message); 
    void error(const std::string& format, const std::string& content);
    void debug(const std::string& message) ;
    void debug(const std::string& format, const std::string& content);

private:
    void log(LogLevel level, const std::string& message); 

    std::string getLogColor(LogLevel level) ;
    std::string formatMessage(LogLevel level, const std::string& message);
};
#endif