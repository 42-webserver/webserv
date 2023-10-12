#include "../../incs/Channel/FileStream.hpp"

void FileStream::inJectChannelFd(int fd) {
    this->setChannelFd(fd);
}

FileStream::FileStream(std::string path, std::string mode) {
    this->_fp = fopen(path.c_str(), mode.c_str());
    if (this->_fp == NULL)
        throw FileStream::FailToOpenException();
    this->inJectChannelFd(fileno(this->_fp));
}
FileStream::~FileStream(void) {
    if (this->_fp != NULL)
        fclose(this->_fp);
}

const char	*FileStream::FailToOpenException::what(void) const throw() { return ("FileStream: Fail to open"); }