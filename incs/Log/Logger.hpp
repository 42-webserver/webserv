#ifndef LOGGER_HPP
# define LOGGER_HPP

# include <fstream>
# include <sstream>
# include <iostream>
# include <ctime>
# include <cstdarg>
# include "../../libs/shared_ptr/shared_ptr.hpp"

// # include <Event/EventQueue/EventQueue.hpp>
// # include <Event/EventBase/EventFactory.hpp>
// Refactoring::daegulee  access, error Expand!
// Todo:1. Loger must be event
// 2. Loger using buf 
// 3. Logger -> 4 state -> Info (Access info write -> Log.txt)
// 						-> Warining (Assertion : function Behavior Broken);
						// -> DeBug (LogDebug.txt)
						// -> Error (LogError.txt)
						// Logger Behavior a) buffer 4개- warning, info, error, debug
// 4. 만약에 디렉토리 없으면 만드는 코드 필요
const std::string DEFAULT_LOG_FILE_NAME = "/Users/idaegyu/Desktop/daeguWeb/Logsave/Log.txt";

typedef enum{
	e_log_cycle,
	e_log_immidiate
}	e_log_save;

static const ssize_t BUFFER_SIZE = 32 * 1024;


class IoReadAndWriteBuffer;
class Logger {
	public:
		class AccessKey
		{
			friend class LogEventHandler;
			friend class Logger;
			private:
				AccessKey();
				~AccessKey();
		};
	private:
		const static std::string	_format[];
		const static std::string	_log_color[];
		ft::shared_ptr<IoReadAndWriteBuffer>	_buffer;
		static Logger	*_instance;
	private:
		// std::ostream	*_output_stream;
		// std::ofstream	_file_stream;
		// bool 			_log_to_stdout;
		std::string		_formatted_message;

	private:
		Logger(void);

	public:
		~Logger(void);

	public:
		static Logger	&getInstance(void);
	private:
		void	_flush(void);

	public:
		// void	info(const std::string& message) ;
		// void	info(const std::string& format, int count, ...);
		// void	warning(const std::string& message) ;
		// void	warning(const std::string& format, int count, ...); 
		void	error(const std::string& message); 
		void	error(const std::string& format, int count, ...);
		ssize_t  getBufferSize(void) const;
		// void	debug(const std::string& message) ;
		// void	debug(const std::string& format, int count, ...);

	private:
		void	log(const std::string &message);
	public:
		void    _onBoardLogEvent(const AccessKey &accessKey);
	private:
		std::string	converformatMessage(const std::string &format, int count, va_list args);
		std::string	formatMessage(const std::string &message);
};

#endif