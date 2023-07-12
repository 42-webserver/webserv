#include "../../incs/Config/AllowMethodElement.hpp"
#include "../../incs/Log/Logger.hpp"

AllowMethodElement::MethodMap	AllowMethodElement::_method_map = AllowMethodElement::_initMethodMap();
AllowMethodElement::MethodMap	AllowMethodElement::_initMethodMap(void) {
	MethodMap	ret;

	ret["GET"] = M_GET;
	ret["POST"] = M_POST;
	ret["DELETE"] = M_DELETE;
	return (ret);
}

AllowMethodElement::AllowMethodElement(void): _flag(M_NONE) {}
AllowMethodElement::AllowMethodElement(std::ifstream &infile) throw(std::exception): _flag(M_NONE) {
	try {
		this->_parse(infile);
	} catch (const std::exception &e) {
		Logger::getInstance().error(e.what());
		throw (FailToParseException());
	}
}
AllowMethodElement::AllowMethodElement(const AllowMethodElement &ref): _flag(ref._flag) {}
AllowMethodElement::~AllowMethodElement(void) {}
AllowMethodElement	&AllowMethodElement::operator=(const AllowMethodElement &rhs) {
	if (this != &rhs) {
		this->~AllowMethodElement();
		new (this) AllowMethodElement(rhs);
	}
	return (*this);
}

bool	AllowMethodElement::_parse(std::ifstream &infile) throw(std::exception) {
	std::string					token;
	MethodMap::const_iterator	it;

	while (infile >> token) {
		if (token.back() == ';') {
			token = token.substr(0, token.length() - 1);
			if ((it = AllowMethodElement::_method_map.find(token)) == AllowMethodElement::_method_map.end()) {
				throw (InvalidArgumentException());
			}
			this->_flag |= it->second;
			break;
		}
		if ((it = AllowMethodElement::_method_map.find(token)) == AllowMethodElement::_method_map.end()) {
			throw (InvalidArgumentException());
		}
		this->_flag |= it->second;
	}
	if (this->_flag == M_NONE) {
		throw (InvalidSyntaxException());
	}
	return (true);
}

int	AllowMethodElement::getFlag(void) const { return (this->_flag); }

const char	*AllowMethodElement::FailToParseException::what(void) const throw() { return ("AllowMethodElement: Fail to Parse"); }
const char	*AllowMethodElement::InvalidSyntaxException::what(void) const throw() { return ("AllowMethodElement: Invalid Syntax"); }
const char	*AllowMethodElement::InvalidArgumentException::what(void) const throw() { return ("AllowMethodElement: Invalid Argument"); }

std::ostream	&operator<<(std::ostream &os, const AllowMethodElement &rhs) {
	os << rhs.getFlag();
	return (os);
}