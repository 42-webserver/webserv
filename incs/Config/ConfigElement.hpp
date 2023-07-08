#ifndef CONFIGELEMENT_HPP
# define CONFIGELEMENT_HPP

# include "../../libs/shared_ptr/shared_ptr.hpp"
# include <exception>
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