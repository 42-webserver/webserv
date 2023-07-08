#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "../../libs/shared_ptr/shared_ptr.hpp"
# include "ConfigElement.hpp"
# include <string>
# include <vector>

class Config {
	public:
		typedef ft::shared_ptr<ConfigElement>	ElementPtr;
		typedef std::vector<ElementPtr> 		ElementPtrVector;

	private:
		std::string			_file_name;
		ElementPtrVector	_element_ptr_vector;
	
	public:
		Config(void);
		Config(const char *file_name);
		Config(const Config &ref);
		~Config(void);
		Config	&operator=(const Config &rhs);

	public:
		const ElementPtrVector	&getElementPtrVector(void) const;

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