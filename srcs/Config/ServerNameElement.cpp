#include "../../incs/Config/ServerNameElement.hpp"
#include "../../incs/Log/Logger.hpp"

ServerNameElement::ServerNameElement(void)
    : _server_names(std::vector<ServerName>()) {}

ServerNameElement::ServerNameElement(std::ifstream &infile) throw(std::exception)
    : _server_names(std::vector<ServerName>()) {
	try {
		this->_parse(infile);
	} catch (const std::exception &e) {
		Logger::getInstance().error(e.what());
		throw (FailToParseException());
	}
}

ServerNameElement::~ServerNameElement(void) {}

// 베이스 클래스(ConfigElement)가 있다면 명시적으로 초기화합니다.
ServerNameElement::ServerNameElement(const ServerNameElement &ref)
    : ConfigElement(ref), _server_names(ref._server_names) {}

ServerNameElement &ServerNameElement::operator=(const ServerNameElement &rhs) {
	if (this != &rhs) {
		this->~ServerNameElement();
		new(this) ServerNameElement(rhs);
	}
	return (*this);
}

bool ServerNameElement::_parse(std::ifstream &infile) throw(std::exception) {
	std::string token;

	while (infile >> token) {
		// 빈 문자열 체크 후, 마지막 문자를 인덱스로 확인
		if (!token.empty() && token[token.size() - 1] == ';') {
			token = token.substr(0, token.size() - 1);
			if (this->_nameIsNotValid(token)) {
				throw (InvalidSyntaxException());
			}
			this->_server_names.push_back(token);
			break;
		}
		if (this->_nameIsNotValid(token)) {
			throw (InvalidSyntaxException());
		}
		this->_server_names.push_back(token);
	}
	if (this->_server_names.empty()) {
		throw (InvalidSyntaxException());
	}
	return (true);
}

bool ServerNameElement::_nameIsNotValid(const std::string &name) {
	return (name.empty());
}

const std::vector<ServerNameElement::ServerName> &ServerNameElement::getServerNames(void) const {
	return (this->_server_names);
}

const char *ServerNameElement::FailToParseException::what(void) const throw() {
	return ("ServerNameElement: Fail to parse");
}

const char *ServerNameElement::InvalidSyntaxException::what(void) const throw() {
	return ("ServerNameElement: Invalid syntax");
}

std::ostream &operator<<(std::ostream &os, const ServerNameElement &rhs) {
	for (std::vector<ServerNameElement::ServerName>::const_iterator it = rhs.getServerNames().begin();
	     it != rhs.getServerNames().end(); ++it) {
		os << *it << " ";
	}
	return (os);
}
