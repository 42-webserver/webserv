#include "../../incs/Config/ListenElement.hpp"
#include "../../incs/Log/Logger.hpp"

ListenElement::ListenElement(void): _ip(""), _port(0) {}

ListenElement::ListenElement(std::ifstream &infile): _ip(""), _port(0) {
    try {
        this->_parse(infile);
    } catch (const std::exception &e) {
        Logger::getInstance().error(e.what());
        throw (FailToParseException());
    }
}

ListenElement::~ListenElement(void) {}

ListenElement::ListenElement(const ListenElement &ref)
    : ConfigElement(ref), _ip(ref._ip), _port(ref._port) {}

ListenElement &ListenElement::operator=(const ListenElement &rhs) {
    if (this != &rhs) {
        this->~ListenElement();
        new(this) ListenElement(rhs);
    }
    return (*this);
}

bool ListenElement::_parse(std::ifstream &infile) throw(std::exception) {
    std::string host;
    size_t pos;

    if (!(infile >> host)) {
        throw (InvalidArgumentException());
    }
    if (host.empty() || host[host.size() - 1] != ';') {
        throw (InvalidSyntaxException());
    }
    // 세미콜론 제거
    host = host.substr(0, host.size() - 1);
    
    pos = host.find(":");
    this->_ip = (pos == std::string::npos) ? "0.0.0.0" : host.substr(0, pos);
    this->_port = (pos == std::string::npos) ? std::atoi(host.c_str()) : std::atoi(host.substr(pos + 1).c_str());
    if (ListenElement::_ipIsNotValid(this->_ip)) {
        throw (InvalidIpException());
    }
    if (ListenElement::_portIsNotValid(this->_port)) {
        throw (InvalidPortException());
    }
    return (true);
}

bool ListenElement::_ipIsNotValid(const Ip &ip) {
    std::string::size_type prev_pos = 0, cur_pos;
    int num;

    for (size_t i = 0; i < 3; i++) {
        cur_pos = ip.find(".", prev_pos);
        if (cur_pos == std::string::npos) {
            return (true);
        }
        num = std::atoi(ip.substr(prev_pos, cur_pos - prev_pos).c_str());
        if (num < 0 || 255 < num) {
            return (true);
        }
        prev_pos = cur_pos + 1;
    }
    num = std::atoi(ip.substr(prev_pos).c_str());
    if (num < 0 || 255 < num) {
        return (true);
    }
    return (false);
}

bool ListenElement::_portIsNotValid(const Port &port) { 
    return (port < 0 || 65536 <= port); 
}

const ListenElement::Ip &ListenElement::getIp(void) const { 
    return (this->_ip); 
}

const ListenElement::Port &ListenElement::getPort(void) const { 
    return (this->_port); 
}

const char *ListenElement::FailToParseException::what(void) const throw() { 
    return ("ListenElement: Fail to parse"); 
}

const char *ListenElement::InvalidArgumentException::what(void) const throw() { 
    return ("ListenElement: Invalid argument"); 
}

const char *ListenElement::InvalidSyntaxException::what(void) const throw() { 
    return ("ListenElement: Invalid syntax"); 
}

const char *ListenElement::InvalidIpException::what(void) const throw() { 
    return ("ListenElement: Invalid IP"); 
}

const char *ListenElement::InvalidPortException::what(void) const throw() { 
    return ("ListenElement: Invalid Port"); 
}

std::ostream &operator<<(std::ostream &os, const ListenElement &rhs) {
    os << rhs.getIp() << ":" << rhs.getPort();
    return (os);
}
