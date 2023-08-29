#ifndef READSOCKETBUFFER_HPP
# define READSOCKETBUFFER_HPP

#include "Buffer.hpp"
#include "LargeNode.hpp"

class ReadSocketbuffer : public BaseBuffer {
private:
	LargeNode _head;
};

#endif