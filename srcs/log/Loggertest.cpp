#include "../../incs/log/logger.hpp"
int main() {
    // Get a reference to the Logger instance
    Logger& logger = Logger::getInstance();

    // Set the log level

    // Log messages
    logger.info("This is an information message");
    logger.warning("This is a warning message");
    logger.error("This is an error message");
    logger.debug("This is a debug message");
	std::cout << "\n\n";
    logger.info("hi format {}!", "This is an information message");
    logger.warning("hi format {}!","This is a warning message");
    logger.error("hi format {}!","This is an error message");
    logger.debug("hi format {}!","This is a debug message");
    return 0;
}
