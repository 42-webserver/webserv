#ifndef IOREADANDWRITEBUFFER_HPP
# define IOREADANDWRITEBUFFER_HPP

#include "BaseBuffer.hpp"
#include "../Node/LargeNode.hpp"
#include "../Node/NormalNode.hpp"
#include "../../../libs/shared_ptr/shared_ptr.hpp"
#include "../../../libs/Library/Optional.hpp"
#include "../../Channel/Socket.hpp"
#include "../../../libs/Library/Assert.hpp"
#include <list>
#include "../Exception/AllocationException.hpp"
#include "IoAble.hpp"
#include "Modifiable.hpp"
class IoReadAndWriteBuffer : public BaseBuffer, IoAble, Modifiable {
private:
    ft::Optional<ft::shared_ptr<NormalNode> >_head;
    std::list<ft::Optional<ft::shared_ptr<LargeNode> > > _lst;
//Buffer Inteface
public:
    virtual size_t size();
    virtual ~IoReadAndWriteBuffer();
//IoAble interface
public:
    virtual size_t	ioRead(int fd);
    virtual size_t	ioWrite(int fd);
//Modifiable interface
public:
    virtual size_t erase(size_t size);
    virtual size_t insert(std::vector<char>::iterator start, std::vector<char>::iterator end);
//IoReadAndWriteBuffer interface
public:
    void	_allocate();
};

#endif