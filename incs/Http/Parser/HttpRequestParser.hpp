#ifndef HTTP_REQUEST_PARSER_HPP
#define HTTP_REQUEST_PARSER_HPP

#include "../Request/HttpRequest.hpp"
#include "../Utils/RouterUtils.hpp"
#include "../Utils/HttpStatus.hpp"
#include "../Exception/BadRequestException.hpp"
#include "../../Config/Config.hpp"
#include "../../Server/VirtualServerManager.hpp"
#include "../../../libs/shared_ptr/shared_ptr.hpp"
#include <Buffer/Buffer/IoOnlyReadBuffer.hpp>
#include "ParsePatternMatcher.hpp"
#include "MultipartRequestBodyHandler.hpp"
#include "RequestBodyHandler.hpp"
#include "NormalBodyHandler.hpp"
#include "ChunkedRequestBodyHandler.hpp"
#include <vector>
#include <iostream>


typedef enum State{
		BEFORE,
		START_LINE,
		HEADERS,
		BODY,
		FINISH
} RequestParseState;


class HttpRequestParser{

public:
class ClientBodySizeInvalidException : public BadRequestException {
public:
	ClientBodySizeInvalidException() : BadRequestException(BAD_REQUEST) {}

	const char* what() const throw(){
		return "Client Body Size Invalid";
	}
};

private:
	ft::shared_ptr<HttpRequest>			_httpRequest;
	RequestParseState					_state;
	std::vector<char>					_buffer;
	ft::shared_ptr<RequestBodyHandler>	_bodyHandler;


public:
	//check: this maybe void
	HttpRequestParser(void);
	const RequestParseState &parseRequest(ft::shared_ptr<VirtualServerManager> vsm);
	void clearBuffer();
	const RequestParseState &getState();
	const int & getReadBodySize();
	const ft::shared_ptr<HttpRequest>	 getHttpRequest();
	const std::vector<char> & getBuffer();
	friend std::ostream &operator<<(std::ostream & os, const HttpRequestParser & parser);

private:
	void handleStartLineState();
	void handleHeaderState(ft::shared_ptr<VirtualServerManager> vsm);
	void handleBodyState();
	bool isFileExists(const std::string& filename);
	std::string generateUniqueFileName();
	void changeStateToBody(ft::shared_ptr<VirtualServerManager> vsm);
	void injectionHandler();
};

#endif