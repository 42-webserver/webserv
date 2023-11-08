#include "../../incs/Config/AliasElement.hpp"
#include "../../incs/Log/Logger.hpp"

AliasElement::AliasElement(void) {}
AliasElement::AliasElement(std::ifstream &infile) throw(std::exception) {
	try {
		this->_parse(infile);
	} catch (const std::exception &e) {
		Logger::getInstance().error(e.what());
		throw (FailToParseException());
	}
}
AliasElement::AliasElement(const AliasElement &ref): _path(ref._path) {}
AliasElement::~AliasElement(void) {}
AliasElement	&AliasElement::operator=(const AliasElement &rhs) {
	if (this != &rhs) {
		this->~AliasElement();
		new (this) AliasElement(rhs);
	}
	return (*this);
}

bool	AliasElement::_parse(std::ifstream &infile) throw(std::exception) {
	std::string	token;

	if (!(infile >> token) && (token.back() != ';')) {
		throw (InvalidSyntaxException());
	}
	this->_path = token.substr(0, token.length() - 1);
	//TO do: check this
	if (this->_path.back() != '/') {
		this->_path.push_back('/');
	}
	if (this->_pathIsNotValid(this->_path)) {
		throw (InvalidArgumentException());
	}
	return (true);
}

bool	AliasElement::_pathIsNotValid(const std::string &path) { return (path[0] != '/'); }

const std::string	&AliasElement::getPath(void) const { return (this->_path); }

const char	*AliasElement::FailToParseException::what(void) const throw() { return ("AliasElement: Fail to Parse"); }
const char	*AliasElement::InvalidSyntaxException::what(void) const throw() { return ("AliasElement: Invalid Syntax"); }
const char	*AliasElement::InvalidArgumentException::what(void) const throw() { return ("AliasElement: Invalid Argument"); }

std::ostream	&operator<<(std::ostream &os, const AliasElement &rhs) {
	os << rhs.getPath();
	return (os);
}