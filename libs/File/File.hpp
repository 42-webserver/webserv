#include <unistd.h>
#include <iostream>

#ifndef FILE_HPP
# define FILE_HPP

class File {
protected:
    int _fd;

public:
    File(int fd) : _fd(fd) {}

    virtual void close() = 0;
    virtual ~File() { close(); }

private:
    File(const File&);
    File& operator=(const File&);
};

// Connection file class
class ConnectionFd : public File {
public:
    ConnectionFd(int fd) : File(fd) {}

    void close() override {
        ::close(_fd); // Close the connection file descriptor
    }

private:
    ConnectionFd(const ConnectionFd&);
    ConnectionFd& operator=(const ConnectionFd&);
};

// File file class
class FileFd : public File {
private:
    bool isReading;
    bool isWriting;

public:
    FileFd(int fd) : File(fd), isReading(false), isWriting(false) {}

    void setReading(bool reading) {
        isReading = reading;
    }

    void setWriting(bool writing) {
        isWriting = writing;
    }

    bool isReadingState() const {
        return isReading;
    }

    bool isWritingState() const {
        return isWriting;
    }

    void close() override {
        ::close(_fd); // Close the file descriptor
    }

private:
    FileFd(const FileFd&);
    FileFd& operator=(const FileFd&);
};

#endif