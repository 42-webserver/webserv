#include "../../incs/Config/AutoIndexElement.hpp"
#include "../../incs/Log/Logger.hpp"

AutoIndexElement::AutoIndexElement(void) {}
AutoIndexElement::AutoIndexElement(std::ifstream &infile) throw(std::exception) {
	try {
		this->_parse(infile);
	} catch (const std::exception &e) {
		Logger::getInstance().error(e.what());
		throw (FailToParseException());
	}
}
AutoIndexElement::AutoIndexElement(const AutoIndexElement &ref): _flag(ref._flag) {}
AutoIndexElement::~AutoIndexElement(void) {}
AutoIndexElement	&AutoIndexElement::operator=(const AutoIndexElement &rhs) {
	if (this != &rhs) {
		this->~AutoIndexElement();
		new (this) AutoIndexElement(rhs);
	}
	return (*this);
}

bool	AutoIndexElement::_parse(std::ifstream &infile) throw(std::exception) {
	std::string	token;

	if (!(infile >> token) && (token.back() != ';')) {
		throw (InvalidSyntaxException());
	}
	token = token.substr(0, token.length() - 1);
	if (this->_flagIsNotValid(token)) {
		throw (InvalidArgumentException());
	}
	this->_flag = (token == "on") ? true : false;
	return (true);
}

bool	AutoIndexElement::_flagIsNotValid(const std::string &flag) { return (flag != "on" && flag != "off"); }

bool	AutoIndexElement::getFlag(void) const { return (this->_flag); }

const char	*AutoIndexElement::FailToParseException::what(void) const throw() { return ("AutoIndexElement: Fail to Parse"); }
const char	*AutoIndexElement::InvalidSyntaxException::what(void) const throw() { return ("AutoIndexElement: Invalid Syntax"); }
const char	*AutoIndexElement::InvalidArgumentException::what(void) const throw() { return ("AutoIndexElement: Invalid Argument"); }