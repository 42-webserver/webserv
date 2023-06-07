#include <unistd.h>
#include <iostream>

#ifndef FILE_HPP
# define FILE_HPP

# include <fcntl.h>
# include "../Log/Logger.hpp"
// file class
class FileDescriptor{
	private:
		int _fd;

	public:
		FileDescriptor(int fd);
		~FileDescriptor(void);
		void setNonBlocking(void) throw (std::exception);

	private:
		FileDescriptor(const FileDescriptor&);
		FileDescriptor& operator=(const FileDescriptor&);

	public:
		int	getFd() const;
	public:
		class FailToNonBlockException: public std::exception {
			public:
				virtual const char *what() const throw();
		};
};

#endif