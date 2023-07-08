#include "../../incs/Server/PhysicalServer.hpp"
#include "../../incs/Log/Logger.hpp"

PhysicalServer::PhysicalServer(void): _socket(ft::shared_ptr<Socket>(NULL)), _virtual_server_manager(ft::shared_ptr<VirtualServerManager>(NULL)) {}
PhysicalServer::~PhysicalServer(void) {}

bool	PhysicalServer::build(const Ip &ip, const ft::shared_ptr<ServerElement> &element) throw(std::exception) {
	try {
		if (this->_virtual_server_manager.get() == NULL) {
			this->_ip = ip;
			this->_virtual_server_manager = ft::shared_ptr<VirtualServerManager>(new VirtualServerManager());
		}
		this->_virtual_server_manager->build(ip, element);
	} catch (const std::exception &e) {
		Logger::getInstance().error(e.what());
		throw (FailToBuildException());
	}
	return (true);
}
bool	PhysicalServer::buildSocket(const Port &port) throw(std::exception) {
	try {
		this->_socket = ft::shared_ptr<Socket>(new Socket());
		this->_socket->build(port, this->_ip);
	} catch (const std::exception &e) {
		Logger::getInstance().error(e.what());
		throw (FailToBuildSocketException());
	}
	return (true);
}
bool	PhysicalServer::run(void) throw(std::exception) {
	try {
		this->_socket->run();
	} catch (const std::exception &e) {
		Logger::getInstance().error(e.what());
		throw (FailToRunException());
	}
	return (true);
}
bool	PhysicalServer::hasServerWithWildCardIp(void) const {
	return (this->_virtual_server_manager->hasServerWithWildCardIp());
}
bool	PhysicalServer::mergeAllVirtualServer(const ft::shared_ptr<PhysicalServer> &other) throw(std::exception) {
	try {
		this->_ip = "0.0.0.0";
		this->_virtual_server_manager->mergeAllVirtualServer(other->_virtual_server_manager);
	} catch (const std::exception &e) {
		Logger::getInstance().error(e.what());
		throw (FailToMergeAllVirtualServerException());
	}
	return (true);
}

ft::shared_ptr<VirtualServer>	PhysicalServer::findVirtualServer(const Ip &ip) const { return (this->_virtual_server_manager->findVirtualServer(ip)); }

const char	*PhysicalServer::FailToBuildException::what() const throw() { return "PhysicalServer: Fail to build"; }
const char	*PhysicalServer::FailToRunException::what() const throw() { return "PhysicalServer: Fail to run"; }
const char	*PhysicalServer::FailToMergeAllVirtualServerException::what() const throw() { return "PhysicalServer: Fail to merge all virtual server"; }
const char	*PhysicalServer::FailToBuildSocketException::what() const throw() { return "PhysicalServer: Fail to build socket"; }

std::ostream	&operator<<(std::ostream &os, const PhysicalServer &physical_server) {
	os << "\t\t\t" << "PhysicalServer:" << '\n';
	os << *(physical_server._socket) << '\n';
	os << *(physical_server._virtual_server_manager);
	return (os);
}