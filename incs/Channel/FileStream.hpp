#ifndef FILESTREAM_HPP
# define FILESTREAM_HPP

#include <stdio.h>
#include "Channel.hpp"
#include <iostream>

class FileStream : public Channel {
private:
    FILE *_fp;
public:
    FileStream(std::string path, std::string mode);
    virtual ~FileStream(void);
    virtual void inJectChannelFd(int fd);
    long getFileSize();
class FailToOpenException: public std::exception {
    public:
        virtual const char *what() const throw();
};
};

#endif