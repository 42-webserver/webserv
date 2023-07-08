#include "../../incs/Config/LocationTrieElement.hpp"
#include "../../incs/Log/Logger.hpp"

LocationTrieElement::LocationTrieElement(void) {}
LocationTrieElement::LocationTrieElement(std::ifstream &infile) throw(std::exception): _location_element_ptr_trie(LocationElementPtrTrie()) {
	try {
		this->_parse(infile);
	} catch (std::exception &e) {
		throw (FailToParseException());
	}
}
LocationTrieElement::~LocationTrieElement(void) {}
LocationTrieElement::LocationTrieElement(const LocationTrieElement &ref): _location_element_ptr_trie(ref._location_element_ptr_trie) {}
LocationTrieElement	&LocationTrieElement::operator=(const LocationTrieElement &rhs) {
	if (this != &rhs) {
		this->~LocationTrieElement();
		new (this) LocationTrieElement(rhs);
	}
	return (*this);
}

bool	LocationTrieElement::insert(std::ifstream &infile) throw(std::exception) {
	std::string	token;

	if (!(infile >> token)) {
		throw (InvalidSyntaxException());
	}
	if (LocationTrieElement::_dirIsNotValid(token)) {
		throw (InvalidArgumentException());
	}
	token = (token.back() != '/') ? token + "/" : token;
	if (this->_location_element_ptr_trie.find(token) != this->_location_element_ptr_trie.end()) {
		throw (DuplicatedDirectoryException());
	}
	try {
		this->_location_element_ptr_trie.insert(token, ft::static_pointer_cast<LocationElement>(ConfigElementFactory::getInstance().create("location", infile)));
	} catch (const std::exception &e) {
		Logger::getInstance().error(e.what());
		throw (FailToInsertException());
	}
	return (true);
}

LocationTrieElement::LocationElementPtr	LocationTrieElement::longestPrefixSearch(const std::string &path) const { return (this->_location_element_ptr_trie.longestPrefixSearch(path)); }

bool	LocationTrieElement::_parse(std::ifstream &infile) throw(std::exception) {
	try {
		this->insert(infile);
	} catch (const std::exception &e) {
		Logger::getInstance().error(e.what());
		throw (FailToInsertException());
	}
	return (true);
}

bool	LocationTrieElement::_dirIsNotValid(const std::string &dir) { return (dir.size() == 0 || dir[0] != '/'); }

const char	*LocationTrieElement::FailToParseException::what(void) const throw() { return ("LocationTrieElement: Fail to parse"); }
const char	*LocationTrieElement::FailToInsertException::what(void) const throw() { return ("LocationTrieElement: Fail to insert"); }
const char	*LocationTrieElement::DuplicatedDirectoryException::what(void) const throw() { return ("LocationTrieElement: Duplicated directory"); }
const char	*LocationTrieElement::InvalidSyntaxException::what(void) const throw() { return ("LocationTrieElement: Invalid syntax"); }
const char	*LocationTrieElement::InvalidArgumentException::what(void) const throw() { return ("LocationTrieElement: Invalid argument"); }