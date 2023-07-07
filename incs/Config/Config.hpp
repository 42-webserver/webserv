#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <string>
# include <vector>

# include "ConfigElement.hpp"

class Config {
	public:
		typedef std::vector<ConfigElement *> ElementVector;

	private:
		std::string		_file_name;
		ElementVector	_element_vector;
	
	public:
		Config(void);
		Config(const char *file_name);
		Config(const Config &ref);
		~Config(void);
		Config	&operator=(const Config &rhs);

	public:
		const ElementVector	&getElementVector(void) const;

	private:
		bool	_parse(void) throw(std::exception);

	private:
		static bool	invalidFileName(const std::string &file_name);

	public:
		class FailToParseException: public std::exception {
			public:
				virtual const char* what() const throw();
		};
		class InvalidFileNameException: public std::exception {
			public:
				virtual const char* what() const throw();
		};
		class FailToOpenFileException: public std::exception {
			public:
				virtual const char* what() const throw();
		};
		class InvalidSyntaxException: public std::exception {
			public:
				virtual const char* what() const throw();
		};
		class InvalidArgumentException: public std::exception {
			public:
				virtual const char* what() const throw();
		};
};

#endif