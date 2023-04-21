#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <vector>
# include <iostream>

class Server {
	private:
		static std::string			_default_err_page;
		int							_port;
		std::vector<std::string>	_names;
		int							_client_max_body_size;
		std::string					_root;
		std::vector<std::string>	_indexes;
		std::string					_cgi_pass;
		//std::string					_cgi_extension;
		//std::string					_cgi_path;
		//TODO: routing rules (location, redirect)

	public:
		Server(void);
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

		// Setters
		bool	setPort(int port) throw(std::exception);
		bool	setClientMaxBodySize(int client_max_body_size) throw(std::exception);
		bool	setRoot(const std::string &root);
		bool	setIndex(const std::string &index);
		bool	setCgiPass(const std::string &cgi_pass);
		//bool	setCgiExtension(const std::string &cgi_extension);
		//bool	setCgiPath(const std::string &cgi_path);

		// Utils
		bool	findName(const std::string &name) const;
		bool	pushName(const std::string &name);
		bool	findIndex(const std::string &index) const;
		bool	pushIndex(const std::string &index);

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