#ifndef FILESTREAM_HPP
# define FILESTREAM_HPP

#include <stdio.h>
#include "Channel.hpp"
#include <iostream>
#include <sys/stat.h>
class FileStream : public Channel {
private:
    FILE *_fp;
public:
    FileStream(std::string path, std::string mode);
    virtual ~FileStream(void);
    virtual void inJectChannelFd(int fd);
    static bool ensure_directory_exists(const std::string& path);
    static FILE* fopen_with_dirs(const std::string& path, const char* mode);
class FailToOpenException: public std::exception {
    public:
        virtual const char *what() const throw();
};
};

#endif