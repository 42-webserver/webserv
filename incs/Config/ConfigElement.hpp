#ifndef CONFIGELEMENT_HPP
# define CONFIGELEMENT_HPP

# include "../../libs/shared_ptr/shared_ptr.hpp"
# include <fstream>

class ConfigElement {
	public:
		ConfigElement(void);
		ConfigElement(std::ifstream &infile) throw(std::exception);
		virtual ~ConfigElement(void);
		ConfigElement(const ConfigElement &ref);
		ConfigElement	&operator=(const ConfigElement &rhs);
	
	protected:
		virtual bool	_parse(std::ifstream &infile) throw(std::exception) = 0;
};

#endif