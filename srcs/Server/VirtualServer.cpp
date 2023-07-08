#include "../../incs/Server/VirtualServer.hpp"
#include "../../incs/Log/Logger.hpp"
#include <unistd.h>
#include <iostream>
#include <errno.h>

VirtualServer::VirtualServer(const ft::shared_ptr<ServerElement> &element) throw(std::exception): _server_element(_parseServerElement(element)) {}
VirtualServer::~VirtualServer(void) {}
VirtualServer::VirtualServer(const VirtualServer &ref): _server_element(ref._server_element) {}
VirtualServer	&VirtualServer::operator=(const VirtualServer &rhs) {
	if (this != &rhs) {
		this->~VirtualServer();
		new (this) VirtualServer(rhs);
	}
	return (*this);
}

const ServerElement	&VirtualServer::getServerElement(void) const { return (this->_server_element); }

ServerElement	VirtualServer::_parseServerElement(const ft::shared_ptr<ServerElement> &element) {
	ServerElement	ret;

	for (ServerElement::const_iterator it=element->begin(); it != element->end(); ++it) {
		if (it->first == ServerElement::KEY::LISTEN) {
			continue;
		}
		ret[it->first] = it->second;
	}
	return (ret);
}

std::ostream	&operator<<(std::ostream &os, const VirtualServer &rhs) {
	os << "\t\t\t\t\t\t\t" << "VirtualServer:" << '\n';
	os << rhs.getServerElement();
	return (os);
}