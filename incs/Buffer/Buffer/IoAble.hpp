#ifndef IOABLE_HPP
# define IOABLE_HPP

#include <iostream>

class IoReadable{

public:
	virtual ssize_t ioRead(int fd) = 0;
};

class IoWritable{
public:
	virtual ssize_t ioWrite(int fd) = 0;
};

class IoAble : public IoReadable, public IoWritable{
public:
	virtual ssize_t ioRead(int fd) = 0;
	virtual ssize_t ioWrite(int fd) = 0;
};
#endif