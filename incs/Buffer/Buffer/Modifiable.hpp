#ifndef MODIFIABLE_HPP
# define MODIFIABLE_HPP


#include <iostream>
#include <vector>

class Modifiable{
public:
    virtual size_t append(std::vector<char>::iterator begin, std::vector<char>::iterator end) = 0;
    virtual size_t append(std::vector<char>::iterator begin, size_t size) = 0;
    virtual size_t eraseFront(size_t size) = 0;
};

#endif