#ifndef BYTESTREAM_HPP
# define BYTESTREAM_HPP

#include "Channel.hpp"

class ByteStream : public Channel {
public:
    ByteStream(int fd);
    virtual ~ByteStream(void);
    virtual void inJectChannelFd(int fd);
};


#endif
