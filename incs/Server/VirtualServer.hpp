#ifndef VIRTUALSERVER_HPP
# define VIRTUALSERVER_HPP

# include "../../libs/shared_ptr/shared_ptr.hpp"
# include "../Config/ServerElement.hpp"
# include <iostream>

class VirtualServer {
	public:
		typedef ft::shared_ptr<ServerElement>	ServerElementPtr;

	private:
		const ServerElement	_server_element;

	public:
		VirtualServer(const ServerElementPtr &element) throw(std::exception);
		~VirtualServer(void);
		VirtualServer(const VirtualServer &ref);
		VirtualServer	&operator=(const VirtualServer &rhs);
	
	public:
		const ServerElement		&getServerElement(void) const;

	private:
		static ServerElement	_parseServerElement(const ServerElementPtr &element);

	public:
		friend std::ostream		&operator<<(std::ostream &os, const VirtualServer &rhs);
};

#endif