#ifndef BUFFERBASENODE_HPP
# define BUFFERBASENODE_HPP

# include <iostream>
# include <vector>
class BaseNode {
private:
    size_t _partialPoint;
	size_t _currentPos;
	size_t _size;
	size_t _capacity;
	std::vector<char> _buffer;
	bool _setToDelete;
protected:
	BaseNode(size_t size);
    const size_t& getPartialPoint() const;
	void setPartialPoint(size_t n);
	const size_t& getCurrentPos() const;
	void setCurrentPos(size_t n);
	size_t erase(std::vector<char>::iterator start, std::vector<char>::iterator end);
	size_t insert(std::vector<char>::iterator start, std::vector<char>::iterator end);
	virtual ~BaseNode();
public:
    virtual size_t size() = 0;
	bool canDelete() const;
};
#endif