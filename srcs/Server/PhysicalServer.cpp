#include "../../incs/Server/PhysicalServer.hpp"
#include "../../incs/Log/Logger.hpp"

PhysicalServer::PhysicalServer(void): _socket(ft::shared_ptr<Socket>(NULL)), _virtual_server_manager(ft::shared_ptr<VirtualServerManager>(NULL)) {}
PhysicalServer::~PhysicalServer(void) {}

bool	PhysicalServer::build(const Ip &ip, const Config::map &config_map) throw(std::exception) {
	try {
		this->_virtual_server_manager->build(ip, config_map);
	} catch (const std::exception &e) {
		Logger::getInstance().error(e.what());
		throw (FailToBuildException());
	}
	return (true);
}
bool	PhysicalServer::run(void) {
	try {
		// TODO
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
		this->_virtual_server_manager->mergeAllVirtualServer(other->_virtual_server_manager);
	} catch (const std::exception &e) {
		Logger::getInstance().error(e.what());
		throw (FailToMergeAllVirtualServerException());
	}
	return (true);
}

const char	*PhysicalServer::FailToBuildException::what() const throw() { return "PhysicalServer: Fail to build"; }
const char	*PhysicalServer::FailToRunException::what() const throw() { return "PhysicalServer: Fail to run"; }
const char	*PhysicalServer::FailToMergeAllVirtualServerException::what() const throw() { return "PhysicalServer: Fail to merge all virtual server"; }