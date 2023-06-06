#ifndef HTTP_REQUEST_PARSER_HPP
#define HTTP_REQUEST_PARSER_HPP

#include "../request/HttpRequest.hpp"
#include <vector> 

typedef enum State{
		BEFORE,
		START_LINE,
		HEADERS,
		BODY
} RequestParseState;

static const char			   _crlf[] = "\r\n";
static const int				_crlfPatternSize = sizeof(_crlf) - 1;
static const std::vector<char>  _crlfPattern = std::vector<char>(_crlf, _crlf + _crlfPatternSize);

class HttpRequestParser{

private:
	HttpRequest					 *_httpRequest;
	RequestParseState			   _state;
	std::vector<char>			   _buffer;
	int							 _readBodySize;

public:
	//check: this maybe void
	HttpRequestParser(void);
	const RequestParseState &parseRequest(std::vector<char> &buffer);
	void clearBuffer();
	const RequestParseState &getState();
	const int & getReadBodySize();
	const HttpRequest &getHttpRequest();
	const std::vector<char> & getBuffer();

private:
	void handleStartLineState(std::vector<char> &buffer);
	void handleHeaderState(std::vector<char> &buffer);
	void handleBodyState(std::vector<char> &buffer);
};

#endif