#include <Log/Logger.hpp>


Logger	*Logger::_instance = NULL;


Logger::Logger(void) { 
}

Logger	&Logger::getInstance(void) {
	if (Logger::_instance == NULL) {
		Logger::_instance = new Logger();
	}
	return (*Logger::_instance);
}

Logger::~Logger(void) {
	_flush();
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



// void	Logger::info(const std::string& message) { log(INFO, message); }
// void	Logger::info(const std::string& format, int count, ...) {
// 	va_list		args;

// 	va_start(args, count);
// 	log(INFO, converformatMessage(format, count, args));
// 	va_end(args);
// }

// void	Logger::warning(const std::string& message) { log(WARNING, message); }
// void	Logger::warning(const std::string& format, int count, ...) {
// 	va_list 	args;

// 	va_start(args, count);
// 	log(WARNING, converformatMessage(format, count, args));
// 	va_end(args);
// 	//
// }

void	Logger::error(const std::string& message) { log(message); }
void	Logger::error(const std::string& format, int count, ...) {
	va_list		args;

	va_start(args, count);
	log(converformatMessage(format, count, args));
	va_end(args);
}

// void	Logger::debug(const std::string& message) { log(message); }
// void	Logger::debug(const std::string& format, int count, ...) {
// 	va_list		args;
// 	std::string	message;

// 	va_start(args, count);
// 	log(DEBUG, converformatMessage(format, count, args));
// 	va_end(args);
// }


void	Logger::_onBoardLogEvent(){
	EventFactory& eventFactory = EventFactory::getInstance();

	EventDto eventDto(this->_buffer, DEFAULT_LOG_FILE_NAME, "w+");
	Event* event = eventFactory.createEvent(ft::FILE_WRITE_EVENT, eventDto);
	event->onboardQueue();
}

void	Logger::log( const std::string& message) {
	this->_formatted_message = formatMessage(message);
	this->_buffer->appendString(this->_formatted_message);
	if (g_log_save == e_log_immidiate) {
		_onBoardLogEvent();
	}
	else {
		if (this->_buffer->size() > BUFFER_SIZE) {
			_onBoardLogEvent();
		}
	}
}

std::string	Logger::formatMessage(const std::string& message) {
	std::ostringstream	formatted_message_stream;
	std::time_t			currentTime = std::time(NULL);
	std::tm				*localTime = std::localtime(&currentTime);

	formatted_message_stream	<< "[" << (localTime->tm_year + 1900) << "-"
								<< std::setfill('0') << std::setw(2) << (localTime->tm_mon + 1) << "-"
								<< std::setfill('0') << std::setw(2) << localTime->tm_mday << " "
								<< std::setfill('0') << std::setw(2) << localTime->tm_hour << ":"
								<< std::setfill('0') << std::setw(2) << localTime->tm_min << ":"
								<< std::setfill('0') << std::setw(2) << localTime->tm_sec << "] "
								 << message << '\n';

	return (formatted_message_stream.str());
}