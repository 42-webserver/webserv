#include <Event/SpecialEvent/LogEventHandler.hpp>
#include <Log/Logger.hpp>

LogEventHandler::LogEventHandler(void) {}
LogEventHandler::~LogEventHandler(void) {}

void LogEventHandler::handleEvent(Event &event) {
    (void)event;

    std::cerr << "LogEventHandler::handleEvent" << std::endl;
    Logger& logger = Logger::getInstance();
    if (logger.getBufferSize() > 0) {
        logger._onBoardLogEvent(Logger::AccessKey());
    }
}