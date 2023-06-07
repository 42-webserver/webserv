#ifndef HTTP_REQUEST_PARSER_HPP
#define HTTP_REQUEST_PARSER_HPP

#include "../request/HttpRequest.hpp"
#include <vector>
#include <iostream>
#include <sstream>
#include <ctime>
#include <fstream>

typedef enum State{
		BEFORE,
		START_LINE,
		HEADERS,
		BODY,
		FINISH
} RequestParseState;

static const char			   	_crlf[] = "\r\n";
static const int				_crlfPatternSize = sizeof(_crlf) - 1;
static const std::vector<char>  _crlfPattern = std::vector<char>(_crlf, _crlf + _crlfPatternSize);
static const int 				_MAX_BODY_MEMORY_SIZE = 10000;
static const std::string		_tmpDir = "../../../tmp/";

class HttpRequestParser{

private:
	HttpRequest			*_httpRequest;
	RequestParseState	_state;
	std::vector<char>	_buffer;
	int				 	_readBodySize;
	std::ofstream 		_bodyFile;

public:
	HttpRequestParser(HttpRequest *httpRequest);
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
	bool IsFileExists(const std::string& filename);
	std::string generateUniqueFileName();
	void writeInFile(std::vector<char> &buffer);
	void writeInMemory(std::vector<char> &buffer);
};

#endif