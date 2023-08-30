#ifndef BUFFERLARGENODE_HPP
#define BUFFERLARGENODE_HPP

#include "BaseNode.hpp"

class LargeNode : public BaseNode
{
private:
	static const size_t _sizeLarge;
public:
	LargeNode();
	virtual ~LargeNode();
private:
	LargeNode(const LargeNode &other);
	LargeNode& operator=(const LargeNode &other);
};

#endif