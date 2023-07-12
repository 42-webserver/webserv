#ifndef CONFIGELEMENTFACTORY_HPP
# define CONFIGELEMENTFACTORY_HPP

# include "../../libs/shared_ptr/shared_ptr.hpp"
# include "ConfigElement.hpp"
# include <fstream>
# include <string>

class ConfigElementFactory {
	private:
		typedef ft::shared_ptr<ConfigElement>	ElementPtr;

	private:
		ConfigElementFactory(void);
		~ConfigElementFactory(void);
		ConfigElementFactory(const ConfigElementFactory &ref);
		ConfigElementFactory	&operator=(const ConfigElementFactory &rhs);
	
	public:
		static ConfigElementFactory	&getInstance(void);

	public:
		ElementPtr	create(const std::string &element, std::ifstream &infile) const throw(std::exception);

	public:
		class InvalidElementException: public std::exception {
			public:
				virtual const char	*what(void) const throw();
		};
};

#endif