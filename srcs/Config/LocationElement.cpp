#include "../../incs/Config/LocationElement.hpp"
#include "../../incs/Config/ConfigElementFactory.hpp"
#include "../../incs/Log/Logger.hpp"

const LocationElement::KeyMap	LocationElement::_key_map = LocationElement::_initKeyMap();
LocationElement::KeyMap	LocationElement::_initKeyMap(void) {
	KeyMap	ret;

	ret["alias"] = KEY::ALIAS;
	ret["allow_method"] = KEY::ALLOW_METHOD;
	ret["autoindex"] = KEY::AUTOINDEX;
	ret["cgi_pass"] = KEY::CGI_PASS;
	ret["error_page"] = KEY::ERROR_PAGE;
	ret["index"] = KEY::INDEX;
	ret["return"] = KEY::RETURN;
	ret["root"] = KEY::ROOT;
	return (ret);
}

LocationElement::LocationElement(void): _element_map(ElementMap()) {}
LocationElement::LocationElement(std::ifstream &infile) throw(std::exception): _element_map(ElementMap()) {
	try {
		this->_parse(infile);
	} catch (const std::exception &e) {
		Logger::getInstance().error(e.what());
		throw (FailToParseException());
	}
}
LocationElement::~LocationElement(void) {}
LocationElement::LocationElement(const LocationElement &ref): ConfigElement(ref), _element_map(ref._element_map) {}
LocationElement	&LocationElement::operator=(const LocationElement &rhs) {
	if (this != &rhs) {
		this->~LocationElement();
		new(this) LocationElement(rhs);
	}
	return (*this);
}

LocationElement::iterator			LocationElement::begin(void) { return (this->_element_map.begin()); }
LocationElement::iterator			LocationElement::end(void) { return (this->_element_map.end()); }
LocationElement::iterator			LocationElement::find(KEY::e_key key) {
	ElementMap::iterator	it = this->_element_map.find(key);

	if (it == this->_element_map.end()) {
		return (this->_element_map.end());
	}
	return (it);
}

LocationElement::const_iterator		LocationElement::begin(void) const { return (this->_element_map.begin()); }
LocationElement::const_iterator		LocationElement::end(void) const { return (this->_element_map.end()); }
LocationElement::const_iterator		LocationElement::find(KEY::e_key key) const {
	ElementMap::const_iterator	it = this->_element_map.find(key);

	if (it == this->_element_map.end()) {
		return (this->_element_map.end());
	}
	return (it);
}

LocationElement::ElementPtr			&LocationElement::operator[](KEY::e_key key) { return (this->_element_map[key]); }
const LocationElement::ElementPtr	&LocationElement::operator[](KEY::e_key key) const { return (this->_element_map.at(key)); }

bool	LocationElement::_parse(std::ifstream &infile) throw(std::exception) {
	std::string				token;
	KeyMap::const_iterator	key_it;
	
	if ((infile >> token) && (token != "{")) {
		throw (InvalidSyntaxException());
	}
	while (infile >> token) {
		if (token == "}") {
			break;
		}
		if ((key_it = this->_key_map.find(token)) == this->_key_map.end()) {
			throw (InvalidElementException());
		}
		if (this->_element_map.find(key_it->second) != this->_element_map.end()) {
			throw (DuplicatedElementException());
		}
		try {
			this->_element_map[key_it->second] = ft::static_pointer_cast<ConfigElement>(ConfigElementFactory::getInstance().create(key_it->first, infile));
		} catch (const std::exception &e) {
			Logger::getInstance().error(e.what());
			throw (FailToCreateElementException());
		}
	}
	return (true);
}

bool	LocationElement::_elementIsNotValid(const std::string &element) { return (element != "auto_index" && element != "error_page" && element != "index" && element != "return" && element != "root"); }

const char	*LocationElement::FailToParseException::what(void) const throw() { return ("LocationElement: Fail to Parse"); }
const char	*LocationElement::InvalidSyntaxException::what(void) const throw() { return ("LocationElement: Invalid Syntax"); }
const char	*LocationElement::InvalidElementException::what(void) const throw() { return ("LocationElement: Invalid Element"); }
const char	*LocationElement::DuplicatedElementException::what(void) const throw() { return ("LocationElement: Duplicated Element"); }
const char	*LocationElement::FailToCreateElementException::what(void) const throw() { return ("LocationElement: Fail to Create Element"); }