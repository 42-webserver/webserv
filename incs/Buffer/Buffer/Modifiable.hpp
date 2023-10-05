#ifndef MODIFIABLE_HPP
# define MODIFIABLE_HPP


#include <iostream>
#include <vector>
#include "IoOnlyReadBuffer.hpp"
#include "../../../libs/Library/Assert.hpp"
#include "../../../libs/Library/Container/_Type.hpp"
template<typename Derived>
class Modifiable
{
public:
    template <typename Iterator>
    size_t append(Iterator start, Iterator end) {
        STATIC_ASSERT((ft::type::is_same<typename Iterator::value_type, char>::value), "Iterator::value_type should be char");
        STATIC_ASSERT((ft::type::is_same<typename std::iterator_traits<Iterator>::iterator_category, std::random_access_iterator_tag>::value), "Iterator should be a random access iterator");

        return static_cast<Derived*>(this)->appendImpl(start, end);
    }
	size_t popFront(size_t size){
		return static_cast<Derived*>(this)->popFrontImpl(data, size);
	}
};

#endif