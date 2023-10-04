#ifndef IOABLE_HPP
# define IOABLE_HPP

#include <iostream>

class IoReadable{

public:
	virtual size_t ioRead(int fd) = 0;
};

class IoWritable{
public:
	virtual size_t ioWrite(int fd) = 0;
};

class IoAble : public IoReadable, public IoWritable{
public:
	virtual size_t ioRead(int fd) = 0;
	virtual size_t ioWrite(int fd) = 0;
};
#endif