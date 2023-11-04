#ifndef NORMALNODE_HPP
# define NORMALNODE_HPP

# include "BaseNode.hpp"
# include <vector>

class NormalNode : public BaseNode
{
public:
	static const ssize_t _sizeNormal;
public:
	NormalNode();
	virtual ~NormalNode();
	virtual bool isFull() const;
private:
	NormalNode(const NormalNode &other);
	NormalNode& operator=(const NormalNode &other);
};

#endif