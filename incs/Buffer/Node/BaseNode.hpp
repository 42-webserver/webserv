#ifndef BUFFERBASENODE_HPP
# define BUFFERBASENODE_HPP

# include <iostream>
# include <vector>
# include <iterator>
#include "../../../incs/Buffer/Exception/DisconnectionException.hpp"
#include <algorithm>
#include "../../../libs/Library/Assert.hpp"
#include <unistd.h>
//responsibilty : Authorization for deleting in Node, only Node, Not iterator
//Behavior : partial Delete가 일어나는 상황 -> 뒤에 부터 지워지는 추상화
//Behavior : erase가 한번이라도 일어난 후에는 insert가 일어나면 안됨 

// class Mode {
// public:
//     Mode();

// 	/**
// 	 * @brief Constructs a Mode object with the specified properties.
// 	 *
// 	 * @param read Specifies whether the mode allows reading. True to allow reading, false otherwise.
// 	 * @param write Specifies whether the mode allows writing. True to allow writing, false otherwise.
// 	 * @param insert Specifies whether the mode allows inserting. True to allow inserting, false otherwise.
// 	 * @param erase Specifies whether the mode allows erasing. True to allow erasing, false otherwise.
// 	 * @param canDelete Specifies whether the mode allows deletion. True to allow deletion, false otherwise.
// 	 */
// public:
// 	Mode(bool read, bool write, bool insert, bool erase, bool canDelete);
//     inline void setMode(bool read, bool write, bool insert, bool erase, bool canDelete);
//     inline void setMode(unsigned char newMode);
// 	inline void setReadMode();
// 	inline void setWriteMode();
// 	inline void setInsertMode();
// 	inline void setEraseMode();
// 	inline void setCanDeleteMode();

//     inline bool checkMode(Mode newMode) const;
    
//     inline bool isReadMode() const;
//     inline bool isWriteMode() const;
//     inline bool isInsertMode() const;
//     inline bool isEraseMode() const;
//     inline bool isCanDeleteMode() const;

//     void displayModes() const;

// private:
//     unsigned char mode : 5;
// };
class IoOnlyReadBuffer;

namespace ft{
	ssize_t _ioWrite(int fd, std::vector<char>& buffer, ssize_t start);
};
class BaseNode {
typedef std::vector<char>::iterator iterator;
typedef std::vector<char>::const_iterator const_iterator;
// private:
// 	Mode	_mode;
private:
	ssize_t _size;
	ssize_t _capacity;
	ssize_t _eraseSize;
	std::vector<char> _buffer;
public:
	const std::vector<char>& getBuffer() const;
	std::vector<char>& getBuffer();
public:
	BaseNode(ssize_t size);
	ssize_t ioRead(int fd);
	ssize_t ioWrite(int fd);
	ssize_t ioReadToRemainigSize(int fd, ssize_t remainingSize);
	ssize_t insertString(std::string::const_iterator start, ssize_t size);
	ssize_t ioSaveWrite(int fd, ssize_t start);
	ssize_t erase(ssize_t n);
	ssize_t insert(std::vector<char>::iterator start, std::vector<char>::iterator end);
	ssize_t insert(std::vector<char>::iterator start, ssize_t size);
	virtual ~BaseNode();
public:
	iterator begin();
	iterator end();
	const_iterator begin() const;
	const_iterator end() const;
    ssize_t size() const;
	virtual bool isFull() const = 0;
	bool isEmpty() const;
	ssize_t copyTo(std::vector<char>& dest);

class AccessKey {
friend class IoOnlyReadBuffer;
private:
	AccessKey();
	~AccessKey();
};
public:
	void reset(AccessKey key);
};
#endif