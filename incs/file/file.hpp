#include <unistd.h>
#include <iostream>

#ifndef FILE_HPP
# define FILE_HPP

// file class
class FdClass{
public:
    FdClass(int fd);
    ~FdClass();
public:
    const int& getFd() const;

private:
    int _fd;
    FdClass(const FdClass&);
    FdClass& operator=(const FdClass&);
};

// File file class

#endif