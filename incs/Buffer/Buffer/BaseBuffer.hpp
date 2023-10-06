#ifndef BASEBUFFER_HPP
# define BASEBUFFER_HPP

#include <cstddef>
class BaseBuffer{
public:
    BaseBuffer();
    virtual ~BaseBuffer();
    virtual size_t  size()=0;
};

#endif