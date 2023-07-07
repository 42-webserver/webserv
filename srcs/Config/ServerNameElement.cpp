#include "../../incs/Config/ServerNameElement.hpp"
#include "../../incs/Log/Logger.hpp"

ServerNameElement::ServerNameElement(void): _server_names(std::vector<ServerName>()) {}
ServerNameElement::ServerNameElement(std::ifstream &infile) throw(std::exception): _server_names(std::vector<ServerName>()) {
	try {
		this->_parse(infile);
	} catch (const std::exception &e) {
		Logger::getInstance().error(e.what());
		throw (FailToParseException());
	}
}
ServerNameElement::~ServerNameElement(void) {}
ServerNameElement::ServerNameElement(const ServerNameElement &ref): _server_names(ref._server_names) {}
ServerNameElement	&ServerNameElement::operator=(const ServerNameElement &rhs) {
	if (this != &rhs) {
		this->~ServerNameElement();
		new(this) ServerNameElement(rhs);
	}
	return (*this);
}

bool	ServerNameElement::_parse(std::ifstream &infile) throw(std::exception) {
	std::string	token;

	while (infile >> token) {
		if (token.back() == ';') {
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

bool	ServerNameElement::_nameIsNotValid(const std::string &name) { return (name.empty()); }

const std::vector<ServerNameElement::ServerName>	&ServerNameElement::getServerNames(void) const { return (this->_server_names); }

const char	*ServerNameElement::FailToParseException::what(void) const throw() { return ("ServerNameElement: Fail to parse"); }
const char	*ServerNameElement::InvalidSyntaxException::what(void) const throw() { return ("ServerNameElement: Invalid syntax"); }