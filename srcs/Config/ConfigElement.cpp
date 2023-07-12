#include "../../incs/Config/ConfigElement.hpp"

ConfigElement::ConfigElement(void) {}
ConfigElement::ConfigElement(std::ifstream &infile) throw(std::exception) { (void) infile; }
ConfigElement::~ConfigElement(void) {}
ConfigElement::ConfigElement(const ConfigElement &ref) { (void) ref; }
ConfigElement	&ConfigElement::operator=(const ConfigElement &rhs) { (void) rhs; return (*this); }