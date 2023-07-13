#include "../../incs/Server/PhysicalServerManager.hpp"
#include "../../incs/Config/ListenElement.hpp"
#include "../../incs/Event/EventQueue/EventQueue.hpp"
#include "../../incs/Event/ListenEvent/ListenEvent.hpp"
#include "../../incs/Log/Logger.hpp"

PhysicalServerManager::PhysicalServerManager(void): _port_map(PortMap()) {}
PhysicalServerManager::~PhysicalServerManager(void) {}
PhysicalServerManager::PhysicalServerManager(const PhysicalServerManager &rhs): _port_map(rhs._port_map) {}
PhysicalServerManager	&PhysicalServerManager::operator=(const PhysicalServerManager &rhs) {
	if (this != &rhs) {
		this->~PhysicalServerManager();
		new (this) PhysicalServerManager(rhs);
	}
	return (*this);
}

bool	PhysicalServerManager::build(const Config &config) throw(std::exception) {
	try {
		const Config::ElementPtrVector	&elements = config.getElementPtrVector();

		for (size_t i=0; i<elements.size(); i++) {
			this->_buildPhysicalServerVector(ft::static_pointer_cast<ServerElement>(elements[i]));
		}
		for (PortMap::iterator it=this->_port_map.begin(); it!=this->_port_map.end(); it++) {
			if (this->_hasServerWithWildCardIp(it) == true) {
				this->_mergeAllPhysicalServer(it);
			}
		}
		this->_buildSocket();
	} catch (const std::exception &e) {
		Logger::getInstance().error(e.what());
		throw (FailToBuildException());
	}
	return (true);
}
bool	PhysicalServerManager::run(void) throw(std::exception) {
	try {
		for (PortMap::iterator port_it=this->_port_map.begin(); port_it!=this->_port_map.end(); port_it++) {
			for (PhysicalServerPtrVector::iterator ps_it=port_it->second.begin(); ps_it!=port_it->second.end(); ps_it++) {
				// runSocket
				(*ps_it)->run();
				// initEventQueue
				// initListenEvent
			}
		}
	} catch (const std::exception &e) {
		Logger::getInstance().error(e.what());
		throw (FailToBuildException());
	}
	return (true);
}

bool	PhysicalServerManager::_buildPhysicalServerVector(const ServerElementPtr &element) throw(std::exception) {
	const ListenElementPtr	&listen_element_ptr = ft::static_pointer_cast<ListenElement>(element->find(ServerElement::KEY::LISTEN)->second);
	const Port				&port = (Port &) listen_element_ptr->getPort();
	const Ip				&ip = (Ip &) listen_element_ptr->getIp();
	PortMap::iterator		it;
	PhysicalServerPtr		physical_server_ptr;

	if ((it = this->_port_map.find(port)) == this->_port_map.end()) {
		it = (this->_port_map.insert(std::make_pair(port, PhysicalServerPtrVector()))).first;
	}
	if ((physical_server_ptr = this->_findPhysicalServer(it, ip)).get() == NULL) {
		physical_server_ptr = ft::make_shared<PhysicalServer>();
		it->second.push_back(physical_server_ptr);
	}
	try {
		physical_server_ptr->build(ip, element);
	} catch (const std::exception &e) {
		Logger::getInstance().error(e.what());
		throw (FailToBuildPhysicalServerVectorException());
	}
	return (true);
}
bool	PhysicalServerManager::_buildSocket(void) throw(std::exception) {
	try {
		for (PortMap::iterator map_it=this->_port_map.begin(); map_it!=this->_port_map.end(); map_it++) {
			for (PhysicalServerPtrVector::iterator ps_it=map_it->second.begin(); ps_it!=map_it->second.end(); ps_it++) {
				(*ps_it)->buildSocket(map_it->first);
			}
		}
	} catch (const std::exception &e) {
		Logger::getInstance().error(e.what());
		throw (FailToBuildSocketException());
	}
	return (true);
}
bool	PhysicalServerManager::_hasServerWithWildCardIp(const PortMap::const_iterator &it) const {
	const PhysicalServerPtrVector	&physical_server_vector = it->second;

	for (size_t i=0; i<physical_server_vector.size(); i++) {
		if (physical_server_vector[i]->hasServerWithWildCardIp() == true) {
			return (true);
		}
	}
	return (false);
}
bool	PhysicalServerManager::_mergeAllPhysicalServer(const PortMap::iterator &it) throw(std::exception) {
	const PhysicalServerPtrVector		&physical_server_vector = it->second;
	PhysicalServerPtr					physical_server_ptr = physical_server_vector[0];

	try {
		for (size_t i=1; i<physical_server_vector.size(); i++) {
			physical_server_ptr->mergeAllVirtualServer(*(physical_server_vector[i]));
		}
		it->second = PhysicalServerPtrVector(1, physical_server_ptr);
	} catch (const std::exception &e) {
		Logger::getInstance().error(e.what());
		throw (FailToMergeAllPhysicalServerException());
	}
	return (true);
}

PhysicalServerManager::PhysicalServerPtr	PhysicalServerManager::_findPhysicalServer(const PortMap::const_iterator &it, const Ip &ip) {
	const PhysicalServerPtrVector		&physical_server_vector = it->second;
	VirtualServerPtr					virtual_server;

	for (size_t i=0; i<physical_server_vector.size(); i++) {
		virtual_server = physical_server_vector[i]->findVirtualServer(ip);
		if (virtual_server.get() != NULL) {
			return (physical_server_vector[i]);
		}
	}
	return (PhysicalServerPtr());
}

const char	*PhysicalServerManager::FailToBuildException::what() const throw() { return "PhysicalServerManager: Fail to build"; }
const char	*PhysicalServerManager::FailToRunException::what() const throw() { return "PhysicalServerManager: Fail to run"; }
const char	*PhysicalServerManager::TooManyServerException::what() const throw() { return ("PhysicalServerManager: Too many servers"); }
const char	*PhysicalServerManager::FailToMergeAllPhysicalServerException::what() const throw() { return ("PhysicalServerManager: Fail to merge all physical server"); }
const char	*PhysicalServerManager::FailToBuildPhysicalServerVectorException::what() const throw() { return ("PhysicalServerManager: Fail to build physical server vector"); }
const char	*PhysicalServerManager::FailToBuildSocketException::what() const throw() { return ("PhysicalServerManager: Fail to build socket"); }

std::ostream	&operator<<(std::ostream &os, const PhysicalServerManager &physical_server_manager) {
	os << "\t" << "PhysicalServerManager:\n";
	for (PhysicalServerManager::PortMap::const_iterator it=physical_server_manager._port_map.begin(); it!=physical_server_manager._port_map.end(); it++) {
		os << "\t\t" << "Port: " << it->first << '\n';
		for (size_t i=0; i<it->second.size(); i++) {
			os << *(it->second[i]);
			if (i + 1 < it->second.size()) {
				os << '\n';
			}
		}
	}
	return (os);
}