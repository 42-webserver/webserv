#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "../data/Data.hpp"

class Client: public Data {
	public:
		Client(void);
		Client(int fd);
		virtual	~Client(void);
};

#endif