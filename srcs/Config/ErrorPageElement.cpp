#include "../../incs/Config/ErrorPageElement.hpp"
#include "../../incs/Log/Logger.hpp"

ErrorPageElement::ErrorPageElement(void) {}
ErrorPageElement::ErrorPageElement(std::ifstream &infile) throw(std::exception) {
	try {
		this->_parse(infile);
	} catch (const std::exception &e) {
		Logger::getInstance().error(e.what());
		throw (FailToParseException());
	}
}
ErrorPageElement::ErrorPageElement(const ErrorPageElement &ref): _code(ref._code), _uri(ref._uri) {}
ErrorPageElement::~ErrorPageElement(void) {}
ErrorPageElement	&ErrorPageElement::operator=(const ErrorPageElement &rhs) {
	if (this != &rhs) {
		this->~ErrorPageElement();
		new (this) ErrorPageElement(rhs);
	}
	return (*this);
}

bool	ErrorPageElement::_parse(std::ifstream &infile) throw(std::exception) {
	std::string	token;

	if (!(infile >> token) && (token.back() != ';')) {
		throw (InvalidSyntaxException());
	}
	token = token.substr(0, token.length() - 1);
	for (size_t i=0; i<token.length(); i++) {
		if (!std::isdigit(token[i])) {
			throw (InvalidArgumentException());
		}
	}
	this->_code = std::atoi(token.c_str());
	if (this->_codeIsNotValid(this->_code)) {
		throw (InvalidArgumentException());
	}
	if (!(infile >> token) && (token.back() != ';')) {
		throw (InvalidSyntaxException());
	}
	this->_uri = token.substr(0, token.length() - 1);
	if (this->_uriIsNotValid(this->_uri)) {
		throw (InvalidArgumentException());
	}
	return (true);
}

bool	ErrorPageElement::_codeIsNotValid(int code) { return (code < 500 || 600 <= code); }
bool	ErrorPageElement::_uriIsNotValid(const std::string &uri) { return (uri[0] != '/'); }

int					ErrorPageElement::getCode(void) const { return (this->_code); }
const std::string	&ErrorPageElement::getUri(void) const { return (this->_uri); }

const char	*ErrorPageElement::FailToParseException::what(void) const throw() { return ("ErrorPageElement: Fail to Parse"); }
const char	*ErrorPageElement::InvalidSyntaxException::what(void) const throw() { return ("ErrorPageElement: Invalid Syntax"); }
const char	*ErrorPageElement::InvalidArgumentException::what(void) const throw() { return ("ErrorPageElement: Invalid Argument"); }