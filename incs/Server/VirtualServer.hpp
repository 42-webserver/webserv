#ifndef SERVER_HPP
# define SERVER_HPP

# include "../Config/ServerElement.hpp"
# include <iostream>
# include <string>
# include <vector>
# include <map>

class VirtualServer {
	private:
		const ServerElement	_server_element;

	public:
		VirtualServer(const ServerElement *element) throw(std::exception);
		~VirtualServer(void);
		VirtualServer(const VirtualServer &ref);
		VirtualServer	&operator=(const VirtualServer &rhs);
	
	public:
		const ServerElement	&getServerElement(void) const;

	private:
		static ServerElement	_parseServerElement(const ServerElement *element);

	public:
		friend std::ostream	&operator<<(std::ostream &os, const VirtualServer &rhs);
};

#endif