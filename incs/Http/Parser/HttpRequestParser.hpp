#ifndef HTTP_REQUEST_PARSER_HPP
#define HTTP_REQUEST_PARSER_HPP

#include "../Request/HttpRequest.hpp"
#include "../Exception/BadRequestException.hpp"
#include "../../Config/Config.hpp"
#include "../../../libs/shared_ptr/shared_ptr.hpp"
#include "../../../libs/unique_ptr/unique_ptr.hpp"
#include "ParsePatternMatcher.hpp"
#include "MultipartRequestBodyHandler.hpp"
#include "RequestBodyHandler.hpp"
#include "NormalBodyHandler.hpp"
#include "ChunkedRequestBodyHandler.hpp"
#include "FileNameGenerator.hpp"
#include <vector>
#include <iostream>


typedef enum State{
		BEFORE,
		START_LINE,
		HEADERS,
		BODY,
		FINISH
} RequestParseState;


//Refactoring::hyunkyle Shared_ptr-void
class HttpRequestParser{

public:
class ClientBodySizeInvalidException : public BadRequestException {
	public:
			virtual const char* what() const throw();
};

private:
	ft::shared_ptr<HttpRequest>			_httpRequest;
	RequestParseState					_state;
	std::vector<char>					_buffer;
	int				 					_readBodySize;
	ft::shared_ptr<RequestBodyHandler>	_bodyHandler;


public:
	//check: this maybe void
	HttpRequestParser(void);
	const RequestParseState &parseRequest(std::vector<char> &reqBuffer, int clientBodyMaxSize);
	void clearBuffer();
	const RequestParseState &getState();
	const int & getReadBodySize();
	const ft::shared_ptr<HttpRequest>	 getHttpRequest();
	const std::vector<char> & getBuffer();

private:
	void handleStartLineState(std::vector<char> &reqBuffer);
	void handleHeaderState(std::vector<char> &reqBuffer, int clientBodyMaxSize);
	void handleBodyState(std::vector<char> &reqBuffer);
	bool isFileExists(const std::string& filename);
	std::string generateUniqueFileName();
	void changeStateToBody(int clientBodyMaxSize) throw(ClientBodySizeInvalidException);
	void injectionHandler();
};

#endif