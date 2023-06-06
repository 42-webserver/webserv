#include "../../incs/Server/PhysicalServer.hpp"

PhysicalServer::PhysicalServer(void): _host(0), _port(0), _socket(ft::shared_ptr<Socket>(NULL)), _virtual_server_map(VirtualServerMap()) {}
PhysicalServer::PhysicalServer(const std::string &host, const int port) throw(std::exception):
	_host(host),
	_port(port),
	_socket(initSocket(_port)),
	_virtual_server_map(VirtualServerMap())
{
	if (hostIsValid(this->_host) == false) {
		throw (InvalidHostException());
	}
	if (portIsValid(this->_port) == false) {
		throw (InvalidPortException());
	}
}
PhysicalServer::~PhysicalServer(void) {}
PhysicalServer::PhysicalServer(const PhysicalServer &ref): _host(ref._host), _port(ref._port), _socket(ref._socket), _virtual_server_map(ref._virtual_server_map) {}
PhysicalServer	&PhysicalServer::operator=(const PhysicalServer &rhs) {
	if (this != &rhs) {
		this->~PhysicalServer();
		new (this) PhysicalServer(rhs);
	}
	return (*this);
}

ft::shared_ptr<Socket>	PhysicalServer::initSocket(int port) throw(std::exception) {
	ft::shared_ptr<Socket>	ret;

	try {
		ret = ft::shared_ptr<Socket>(new Socket(port));
	} catch (const std::exception &e) {
		std::cerr << "\033[31m" << "Error: " << e.what() << "\033[0m" << '\n';
		throw (PhysicalServer::FailToCreateSocketException());
	}
	return (ret);
}

bool					PhysicalServer::hostIsValid(const std::string &host) {
	bool					ret = true;
	std::string::size_type	prev_pos = 0;
	std::string::size_type	cur_pos = 0;
	size_t					number_of_dots = 0;

	while (ret && (cur_pos = host.find('.', prev_pos)) != std::string::npos) {
		ret = (std::atoi(host.substr(prev_pos, cur_pos - prev_pos).c_str()) < 256);
		number_of_dots++;
		prev_pos = cur_pos + 1;
	}
	ret = ret && (number_of_dots == 3) && (std::atoi(host.substr(prev_pos).c_str()) < 256);

	return (ret);
}
bool					PhysicalServer::portIsValid(int port) { return (0 <= port && port <= 65535); }

void									PhysicalServer::addVirtualServer(const Config::map &config_map) throw(std::exception){
	const std::vector<std::string>	server_names = config_map.at(Config::KEYS[1]);

	for (size_t i=0; i<server_names.size(); i++) {
		if (this->_virtual_server_map.find(server_names[i]) != this->_virtual_server_map.end()) {
			throw (PhysicalServer::DuplicatedVirtualServerException());
		}
		this->_virtual_server_map[server_names[i]] = ft::shared_ptr<VirtualServer>(new VirtualServer(config_map));
	}
}

ft::shared_ptr<Socket>					PhysicalServer::getSocket(void) const { return (this->_socket); }
// const PhysicalServer::VirtualServerMap	*PhysicalServer::getVirtualServerMap(void) const { return &(this->_virtual_server_map); }

const char	*PhysicalServer::InvalidHostException::what(void) const throw() { return ("PhysicalServer: Invalid host"); }
const char	*PhysicalServer::InvalidPortException::what(void) const throw() { return ("PhysicalServer: Invalid port"); }
const char	*PhysicalServer::FailToCreateSocketException::what(void) const throw() { return ("PhysicalServer: Fail to create socket"); }
const char	*PhysicalServer::DuplicatedVirtualServerException::what(void) const throw() { return ("PhysicalServer: Duplicated virtual server"); }

// To do:: getter 없애서 다시 구현 필요 

// std::ostream	&operator<<(std::ostream &os, const PhysicalServer &ref) {
// 	const PhysicalServer::VirtualServerMap				virtual_server_map = *ref.getVirtualServerMap();

// 	for (PhysicalServer::VirtualServerMap::const_iterator it=virtual_server_map.begin(); it!=virtual_server_map.end(); it++) {
// 		os << "Virtual Server name: " << it->first << '\n';
// 		os << *(it->second);
// 		os << '\n';
// 	}
// 	return (os);
// }