#ifndef FILESTREAM_HPP
# define FILESTREAM_HPP

#include "Channel.hpp"
#include <fstream>

class FileStream : public Channel {
private:
    std::fstream _fileStream;
public:
    FileStream(void);
    virtual ~FileStream(void);
};

#endif
