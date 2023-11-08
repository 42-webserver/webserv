#include "../../incs/Config/RootElement.hpp"
#include "../../incs/Log/Logger.hpp"

RootElement::RootElement(void) {}
RootElement::RootElement(std::ifstream &infile) throw(std::exception) {
	try {
		this->_parse(infile);
	} catch (const std::exception &e) {
		Logger::getInstance().error(e.what());
		throw (FailToParseException());
	}
}
RootElement::RootElement(const RootElement &ref): _path(ref._path) {}
RootElement::~RootElement(void) {}
RootElement	&RootElement::operator=(const RootElement &rhs) {
	if (this != &rhs) {
		this->~RootElement();
		new (this) RootElement(rhs);
	}
	return (*this);
}

bool	RootElement::_parse(std::ifstream &infile) throw(std::exception) {
	std::string	token;

	if (!(infile >> token) && (token.back() != ';')) {
		throw (InvalidSyntaxException());
	}
	this->_path = token.substr(0, token.length() - 1);
	//TO do: check this
	if (this->_path.back() == '/') {
		this->_path.pop_back();
	}
	if (this->_pathIsNotValid(this->_path)) {
		throw (InvalidArgumentException());
	}	
	return (true);
}

bool	RootElement::_pathIsNotValid(const std::string &path) { return (path[0] != '/'); }

const std::string	&RootElement::getPath(void) const { return (this->_path); }

const char	*RootElement::FailToParseException::what(void) const throw() { return ("RootElement: Fail to Parse"); }
const char	*RootElement::InvalidSyntaxException::what(void) const throw() { return ("RootElement: Invalid Syntax"); }
const char	*RootElement::InvalidArgumentException::what(void) const throw() { return ("RootElement: Invalid Argument"); }

std::ostream	&operator<<(std::ostream &os, const RootElement &rhs) {
	os << rhs.getPath();
	return (os);
}