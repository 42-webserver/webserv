#ifndef BUFREADHANDLER_HPP
# define BUFREADHANDLER_HPP

# include "ReadEvent/ReadEvent.hpp"

class BufReadHandler {
	public:
		BufReadHandler(int fd, const int buf_size);
		~BufReadHandler(void);

	private:
		int	_fd;
		const int	_buf_size;

	private:
		BufReadHandler(const BufReadHandler&);
		BufReadHandler& operator=(const BufReadHandler&);

	public:
		std::vector<char>	readBuf(void) throw(std::exception);
	public:
		class FailToReadNonBlockException: public std::exception {
			public:
				virtual const char* what() const throw();
		};
};

#endif