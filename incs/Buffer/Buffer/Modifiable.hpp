#ifndef MODIFIABLE_HPP
# define MODIFIABLE_HPP


#include <iostream>
#include <vector>

class Modifiable{
public:
    virtual ssize_t append(std::vector<char>::iterator begin, std::vector<char>::iterator end) = 0;
    // virtual ssize_t append(std::vector<char>::iterator begin, ssize_t size) = 0;
    virtual ssize_t eraseFront(ssize_t size) = 0;
};

#endif