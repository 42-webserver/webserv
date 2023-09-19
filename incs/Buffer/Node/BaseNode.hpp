#ifndef BUFFERBASENODE_HPP
# define BUFFERBASENODE_HPP

# include <iostream>
# include <vector>
# include <iterator>

//responsibilty : Authorization for deleting in Node, only Node, Not iterator
//Behavior : partial Delete가 일어나는 상황 -> 뒤에 부터 지워지는 추상화
//Behavior : erase가 한번이라도 일어난 후에는 insert가 일어나면 안됨 

class Mode {
public:
    Mode();

	/**
	 * @brief Constructs a Mode object with the specified properties.
	 *
	 * @param read Specifies whether the mode allows reading. True to allow reading, false otherwise.
	 * @param write Specifies whether the mode allows writing. True to allow writing, false otherwise.
	 * @param insert Specifies whether the mode allows inserting. True to allow inserting, false otherwise.
	 * @param erase Specifies whether the mode allows erasing. True to allow erasing, false otherwise.
	 * @param canDelete Specifies whether the mode allows deletion. True to allow deletion, false otherwise.
	 */
	Mode(bool read, bool write, bool insert, bool erase, bool canDelete);
    inline void setMode(bool read, bool write, bool insert, bool erase, bool canDelete);
    inline void setMode(unsigned char newMode);
	inline void setReadMode();
	inline void setWriteMode();
	inline void setInsertMode();
	inline void setEraseMode();
	inline void setCanDeleteMode();

    inline bool checkMode(Mode newMode) const;
    
    inline bool isReadMode() const;
    inline bool isWriteMode() const;
    inline bool isInsertMode() const;
    inline bool isEraseMode() const;
    inline bool isCanDeleteMode() const;

    void displayModes() const;

private:
    unsigned char mode : 5;
};

class BaseNode {
private:
	Mode	_mode;
private:
    size_t _deleteCount;
	size_t _size;
	size_t _capacity;
	std::vector<char> _buffer;
public:
	const std::vector<char>& getBuffer() const;
	std::vector<char>& getBuffer();
public:
	BaseNode(size_t size);
	size_t ioRead(int fd);
	size_t erase(size_t n);
	size_t insert(std::vector<char>::iterator start, std::vector<char>::iterator end);
	virtual ~BaseNode();
public:
	void	reset();
    size_t size() const;
	bool canDelete() const;
};
#endif