#include "../../incs/Config/ClientMaxBodySizeElement.hpp"
#include "../../incs/Log/Logger.hpp"

ClientMaxBodySizeElement::ClientMaxBodySizeElement(void) {}
ClientMaxBodySizeElement::ClientMaxBodySizeElement(std::ifstream &infile) throw(std::exception) {
	try {
		this->_parse(infile);
	} catch (const std::exception &e) {
		Logger::getInstance().error(e.what());
		throw (FailToParseException());
	}
}
ClientMaxBodySizeElement::ClientMaxBodySizeElement(const ClientMaxBodySizeElement &ref): _num(ref._num), _unit(ref._unit) {}
ClientMaxBodySizeElement::~ClientMaxBodySizeElement(void) {}
ClientMaxBodySizeElement	&ClientMaxBodySizeElement::operator=(const ClientMaxBodySizeElement &rhs) {
	if (this != &rhs) {
		this->~ClientMaxBodySizeElement();
		new (this) ClientMaxBodySizeElement(rhs);
	}
	return (*this);
}

bool	ClientMaxBodySizeElement::_parse(std::ifstream &infile) throw(std::exception) {
	std::string	token;

	if (!(infile >> token) && token.back() != ';') {
		throw (InvalidSyntaxException());
	}
	token = token.substr(0, token.length() - 1);
	this->_num = ('0' <= token.back() && token.back() <= '9') ? std::atoi(token.c_str()) : std::atoi(token.substr(0, token.size() - 1).c_str());
	if (this->_numIsNotValid(this->_num)) {
		throw (InvalidArgumentException());
	}
	this->_unit = ('0' <= token.back() && token.back() <= '9') ? 'B' : token.back();
	if (this->_unitIsNotValid(this->_unit)) {
		throw (InvalidArgumentException());
	}
	return (true);
}

bool	ClientMaxBodySizeElement::_numIsNotValid(int num) { return (num < 0 || 999 < num); }
bool	ClientMaxBodySizeElement::_unitIsNotValid(char unit) { return (unit != 'B' && unit != 'K' && unit != 'M' && unit != 'G'); }

int		ClientMaxBodySizeElement::getNum(void) const { return (this->_num); }
char	ClientMaxBodySizeElement::getUnit(void) const { return (this->_unit); }

const char	*ClientMaxBodySizeElement::FailToParseException::what(void) const throw() { return ("ClientMaxBodySizeElement: Fail to Parse"); }
const char	*ClientMaxBodySizeElement::InvalidSyntaxException::what(void) const throw() { return ("ClientMaxBodySizeElement: Invalid Syntax"); }
const char	*ClientMaxBodySizeElement::InvalidArgumentException::what(void) const throw() { return ("ClientMaxBodySizeElement: Invalid Argument"); }

std::ostream	&operator<<(std::ostream &os, const ClientMaxBodySizeElement &rhs) {
	os << rhs.getNum() << rhs.getUnit();
	return (os);
}