#include "../../incs/Server/PhysicalServer.hpp"
#include "../../incs/Log/Logger.hpp"
#include "../../incs/Event/EventQueue/EventQueue.hpp"
#include "../../incs/Event/ListenEvent/ListenEvent.hpp"
#include "../../incs/Log/Logger.hpp"
#include "../../incs/Event/EventBase/EventFactory.hpp"
#include "../../incs/Event/EventDto/EventDto.hpp"

PhysicalServer::PhysicalServer(void): _socket_ptr(), _virtual_server_manager_ptr() {}
PhysicalServer::~PhysicalServer(void) {}

bool	PhysicalServer::build(const Ip &ip, const ServerElementPtr &element) throw(std::exception) {
	try {
		if (this->_virtual_server_manager_ptr.get() == NULL) {
			this->_ip = ip;
			this->_virtual_server_manager_ptr = ft::make_shared<VirtualServerManager>();
		}
		this->_virtual_server_manager_ptr->build(ip, element);
	} catch (const std::exception &e) {
		Logger::getInstance().error(e.what());
		throw (FailToBuildException());
	}
	return (true);
}
bool	PhysicalServer::buildSocket(const Port &port) throw(std::exception) {
	try {
		this->_socket_ptr = ft::make_shared<Socket>();
		this->_socket_ptr->build(port, this->_ip);
	} catch (const std::exception &e) {
		Logger::getInstance().error(e.what());
		throw (FailToBuildSocketException());
	}
	return (true);
}
bool	PhysicalServer::run(void) throw(std::exception) {
	try {
		EventFactory &event_factory = EventFactory::getInstance();
		this->_socket_ptr->run();
		ft::shared_ptr<Channel> channel = ft::static_pointer_cast<Channel>(this->_socket_ptr);
		EventDto event_dto(channel, this->_virtual_server_manager_ptr);
		Event*listen_event = event_factory.createEvent(ft::LISTEN_EVENT, event_dto);
		listen_event->onboardQueue();
	} catch (const std::exception &e) {
		Logger::getInstance().error(e.what());
		throw (FailToRunException());
	}
	return (true);
}
bool	PhysicalServer::hasServerWithWildCardIp(void) const {
	return (this->_virtual_server_manager_ptr->hasServerWithWildCardIp());
}
bool	PhysicalServer::mergeAllVirtualServer(const PhysicalServer &other) throw(std::exception) {
	try {
		this->_ip = "0.0.0.0";
		this->_virtual_server_manager_ptr->mergeAllVirtualServer(*(other._virtual_server_manager_ptr));
	} catch (const std::exception &e) {
		Logger::getInstance().error(e.what());
		throw (FailToMergeAllVirtualServerException());
	}
	return (true);
}

PhysicalServer::VirtualServerPtr	PhysicalServer::findVirtualServer(const Ip &ip) const { return (this->_virtual_server_manager_ptr->findVirtualServer(ip)); }
//fix Daegulee
PhysicalServer::SocketPtr			PhysicalServer::getSocket(void) const { return (this->_socket_ptr); }
ft::shared_ptr<VirtualServerManager>	PhysicalServer::getVirtualServerManager(void) const { return (this->_virtual_server_manager_ptr); }
const char	*PhysicalServer::FailToBuildException::what() const throw() { return "PhysicalServer: Fail to build"; }
const char	*PhysicalServer::FailToRunException::what() const throw() { return "PhysicalServer: Fail to run"; }
const char	*PhysicalServer::FailToMergeAllVirtualServerException::what() const throw() { return "PhysicalServer: Fail to merge all virtual server"; }
const char	*PhysicalServer::FailToBuildSocketException::what() const throw() { return "PhysicalServer: Fail to build socket"; }

std::ostream	&operator<<(std::ostream &os, const PhysicalServer &physical_server) {
	os << "\t\t\t" << "PhysicalServer:" << '\n';
	os << *(physical_server._socket_ptr) << '\n';
	os << *(physical_server._virtual_server_manager_ptr);
	return (os);
}