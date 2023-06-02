#include <unistd.h>
#include <iostream>

#ifndef FILE_HPP
# define FILE_HPP

// file class
class FileDescriptor{
	private:
		int _fd;

	public:
		FileDescriptor(void);
		FileDescriptor(int fd);
		~FileDescriptor(void);

	private:
		FileDescriptor(const FileDescriptor&);
		FileDescriptor& operator=(const FileDescriptor&);

	public:
		int	getFd() const;
};

#endif