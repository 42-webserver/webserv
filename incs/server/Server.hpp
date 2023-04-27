#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <vector>
# include <iostream>
# include <map>

class Server {
	private:
		const static std::string		_default_err_page;
		const int						_port;
		const std::vector<std::string>	_names;
		const int						_client_max_body_size;
		const std::string				_root;
		const std::vector<std::string>	_indexes;
		const std::string				_cgi_pass;
		//std::string					_cgi_extension;
		//std::string					_cgi_path;
		//TODO: routing rules (location, redirect)

	public:
		Server(const std::map< std::string, std::vector< std::string > > &config_map);
		~Server(void);

		// Getters
		static const std::string		&getDefaultErrPage(void);
		int								getPort(void) const;
		const std::vector<std::string>	&getNames(void) const;
		int								getClientMaxBodySize(void) const;
		const std::string				&getRoot(void) const;
		const std::vector<std::string>	&getIndexes(void) const;
		const std::string				&getCgiPass(void) const;
		//const std::string				&getCgiExtension(void) const;
		//const std::string				&getCgiPath(void) const;

		// Exception
		class InvalidPortException : public std::exception {
			public:
				virtual const char *what() const throw();
		};

		class InvalidClientMaxBodySizeException : public std::exception {
			public:
				virtual const char *what() const throw();
		};
};

std::ostream	&operator<<(std::ostream &os, const Server &rhs);

#endif