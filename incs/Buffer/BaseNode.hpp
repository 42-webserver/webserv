#ifndef BUFFERBASENODE_HPP
# define BUFFERBASENODE_HPP

# include <iostream>
# include <vector>
# include <iterator>
//responsibilty : Authorization for deleting in Node, only Node, Not iterator
//Behavior : partial Delete가 일어나는 상황 -> 뒤에 부터 지워지는 추상화
//Behavior : erase가 한번이라도 일어난 후에는 insert가 일어나면 안됨 
class BaseNode {
private:
    size_t _deleteCount;
	size_t _size;
	size_t _capacity;
	std::vector<char> _buffer;
	bool _setToDelete;
	bool _hasBeenErased;
protected:
	BaseNode(size_t size);;
	size_t erase(size_t n);
	size_t insert(std::vector<char>::iterator start, std::vector<char>::iterator end);
	virtual ~BaseNode();
public:
    size_t size() const;
	bool canDelete() const;
};
#endif