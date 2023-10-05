#ifndef BASEBUFFER_HPP
# define BASEBUFFER_HPP

#include <vector>

class BaseBuffer{
private:
    size_t  _size;
    bool  _blocked;
private:
    
public:
    //every Node need erase,insert maybe except Read Socekt buf
   
    virtual ~BaseBuffer();
    virtual size_t  size()=0;
};

#endif