#include "../../incs/Config/CgiPassElement.hpp"
#include "../../incs/Log/Logger.hpp"

CgiPassElement::CgiPassElement(void) {}
CgiPassElement::CgiPassElement(std::ifstream &infile) throw(std::exception) {
	try {
		this->_parse(infile);
	} catch (const std::exception &e) {
		Logger::getInstance().error(e.what());
		throw (FailToParseException());
	}
}
CgiPassElement::CgiPassElement(const CgiPassElement &ref): _flag(ref._flag) {}
CgiPassElement::~CgiPassElement(void) {}
CgiPassElement	&CgiPassElement::operator=(const CgiPassElement &rhs) {
	if (this != &rhs) {
		this->~CgiPassElement();
		new (this) CgiPassElement(rhs);
	}
	return (*this);
}

bool	CgiPassElement::_parse(std::ifstream &infile) throw(std::exception) {
	std::string	token;

	if (!(infile >> token) && (token.back() != ';')) {
		throw (InvalidSyntaxException());
	}
	this->_flag = token.substr(0, token.length() - 1);
	if (this->_flagIsNotValid(this->_flag)) {
		throw (InvalidArgumentException());
	}
	return (true);
}

bool	CgiPassElement::_flagIsNotValid(const std::string &flag) { return (flag != "on" && flag != "off"); }

const std::string	&CgiPassElement::getFlag(void) const { return (this->_flag); }

const char	*CgiPassElement::FailToParseException::what(void) const throw() { return ("CgiPassElement: Fail to Parse"); }
const char	*CgiPassElement::InvalidSyntaxException::what(void) const throw() { return ("CgiPassElement: Invalid Syntax"); }
const char	*CgiPassElement::InvalidArgumentException::what(void) const throw() { return ("CgiPassElement: Invalid Argument"); }

std::ostream	&operator<<(std::ostream &os, const CgiPassElement &rhs) {
	os << rhs.getFlag();
	return (os);
}