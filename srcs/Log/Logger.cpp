#include "../../incs/Log/Logger.hpp"

const std::string	Logger::_format[] = {
	"[INFO] ",
	"[WARNING] ",
	"[ERROR] ",
	"[DEBUG] "
};

const std::string	Logger::_log_color[] = {
	"\033[0;37m", // INFO
	"\033[0;33m", // WARNING
	"\033[0;31m", // ERROR
	"\033[0;32m"  // DEBUG
};

Logger::Logger(void) {
	this->_file_stream.open(DEFAULT_LOG_FILE_NAME.c_str(), std::ofstream::out | std::ofstream::app);
	if (!this->_file_stream) {
		std::cerr << "Error: Logger: Fail to open file" << '\n';
		this->_output_stream = &std::cout;
	} else {
		this->_output_stream = &this->_file_stream;
	}

	this->_log_to_stdout = false;
	#ifdef DEBUG_FLAG
		this->_log_to_stdout = true;
	#endif
}

Logger	&Logger::getInstance(void) {
	static Logger	instance;

	return (instance);
}

Logger::~Logger(void) {
	if (this->_output_stream != &std::cout && this->_file_stream.is_open()) {
		this->_file_stream.close();
	}
}

std::string Logger::converformatMessage(const std::string& format, int count, va_list args) {
	std::string	message = format;

	for(int i=0; i<count; i++) {
		size_t	pos = message.find("{}");

		if (pos != std::string::npos) {
			std::string	content = va_arg(args, char *);

			message.replace(pos, 2, content);
		}
	}

	return (message);
}



void	Logger::info(const std::string& message) { log(INFO, message); }
void	Logger::info(const std::string& format, int count, ...) {
	va_list		args;

	va_start(args, count);
	log(INFO, converformatMessage(format, count, args));
	va_end(args);
}

void	Logger::warning(const std::string& message) { log(WARNING, message); }
void	Logger::warning(const std::string& format, int count, ...) {
	va_list 	args;

	va_start(args, count);
	log(WARNING, converformatMessage(format, count, args));
	va_end(args);
	//
}

void	Logger::error(const std::string& message) { log(ERROR, message); }
void	Logger::error(const std::string& format, int count, ...) {
	va_list		args;

	va_start(args, count);
	log(ERROR, converformatMessage(format, count, args));
	va_end(args);
}

void	Logger::debug(const std::string& message) { log(DEBUG, message); }
void	Logger::debug(const std::string& format, int count, ...) {
	va_list		args;
	std::string	message;

	va_start(args, count);
	log(DEBUG, converformatMessage(format, count, args));
	va_end(args);
}


void	Logger::log(LogLevel level, const std::string& message) {
	this->_formatted_message = formatMessage(level, message);
	if (this->_log_to_stdout) {
		std::cout << Logger::_log_color[level] << this->_formatted_message << "\033[0m";
	}

	if (this->_output_stream && this->_output_stream != &std::cout) {
		(*this->_output_stream) << this->_formatted_message;
		(*this->_output_stream).flush();
	}
}

std::string	Logger::formatMessage(LogLevel level, const std::string& message) {
	std::ostringstream	formatted_message_stream;
	std::time_t			currentTime = std::time(NULL);
	std::tm				*localTime = std::localtime(&currentTime);

	formatted_message_stream	<< "[" << (localTime->tm_year + 1900) << "-"
								<< std::setfill('0') << std::setw(2) << (localTime->tm_mon + 1) << "-"
								<< std::setfill('0') << std::setw(2) << localTime->tm_mday << " "
								<< std::setfill('0') << std::setw(2) << localTime->tm_hour << ":"
								<< std::setfill('0') << std::setw(2) << localTime->tm_min << ":"
								<< std::setfill('0') << std::setw(2) << localTime->tm_sec << "] "
								<< Logger::_format[level] << message << '\n';

	return (formatted_message_stream.str());
}