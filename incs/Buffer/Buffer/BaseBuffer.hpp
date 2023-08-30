#ifndef BASEBUFFER_HPP
# define BASEBUFFER_HPP

#include <vector>

class BaseBuffer{
private:
    size_t  _size;
    bool  _blocked;
public:
    //every Node need erase,insert maybe except Read Socekt buf
    // virtual size_t erase(size_t size);
	// virtual size_t insert(std::vector<char>::iterator start, std::vector<char>::iterator end) = 0;
    virtual ~BaseBuffer();
    virtual size_t  size()=0;
};

#endif