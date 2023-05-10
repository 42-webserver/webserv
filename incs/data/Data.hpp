#ifndef DATA_HPP
# define DATA_HPP

# include "../../libs/shared_ptr/incs/shared_ptr.hpp"
# include "../socket/Socket.hpp"

class Data {
	protected:
		ft::shared_ptr<Socket>	_socket;

	public:
		Data(void);
		Data(int fd);
		Data(const Data &ref);
		virtual	~Data(void);
		Data	&operator=(const Data &rhs);
	
	public:
		const ft::shared_ptr<Socket>	&getSocket(void) const;
};

#endif