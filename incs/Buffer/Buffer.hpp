#ifndef BUFFER_HPP
# define BUFFER_HPP
#include <vector>

class BaseBuffer{
private:
    size_t  _size;
    bool  _blocked;
public:
    //every Node need erase maybe except Read Socekt buf
	virtual size_t insert(std::vector<char>::iterator start, std::vector<char>::iterator end) = 0;
    virtual ~BaseBuffer();
    virtual size_t  size()=0;
};
#endif