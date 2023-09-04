#ifndef MODIFIABLE_HPP
# define MODIFIABLE_HPP


#include <iostream>
#include <vector>

class Modifiable{

public:
	virtual size_t erase(size_t size) = 0;
	virtual size_t insert(std::vector<char>::iterator start, std::vector<char>::iterator end) = 0;
};

#endif