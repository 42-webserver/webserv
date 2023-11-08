#ifndef BASEBUFFER_HPP
# define BASEBUFFER_HPP

#include <cstddef>
#include <iostream>
class BaseBuffer{
public:
    BaseBuffer();
    virtual ~BaseBuffer();
    virtual ssize_t  size()=0;
};

#endif