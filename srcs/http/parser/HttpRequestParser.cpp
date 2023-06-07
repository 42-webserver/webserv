#include "../../../incs/http/parser/HttpRequestParser.hpp"


//check: this maybe void
HttpRequestParser::HttpRequestParser(void)
: _httpRequest(NULL), _state(BEFORE), _readBodySize(0)
{
}

const RequestParseState &HttpRequestParser::parseRequest(std::vector<char> &buffer) {
	if (!this->_buffer.empty()){
		buffer.insert(buffer.begin(), this->_buffer.begin(), this->_buffer.end());
		this->_buffer.clear();
	}
	if (_state == BEFORE || _state == START_LINE)
		handleStartLineState(buffer);
	if (_state == HEADERS)
		handleHeaderState(buffer);
	if (_state == BODY)
		handleBodyState(buffer);
	return this->_state;
}

void HttpRequestParser::handleStartLineState(std::vector<char> &buffer) {
	if (buffer.empty())
		return;
	std::vector<char>::iterator find = std::search(buffer.begin(), buffer.end(), _crlfPattern.begin(), _crlfPattern.end());
	if (find == buffer.end())
	{
		this->_state = START_LINE;
		this->_buffer.insert(this->_buffer.end(), buffer.begin(), buffer.end());
		return;
	}
	_httpRequest->setStartLine(std::string(buffer.begin(), find));
	buffer.erase(buffer.begin(), find + _crlfPatternSize);
	this->_state = HEADERS;
}

void HttpRequestParser::handleHeaderState(std::vector<char> &buffer) {
	if (buffer.empty())
		return;
	std::string line;
	std::vector<char>::iterator find = std::search(buffer.begin(), buffer.end(), _crlfPattern.begin(), _crlfPattern.end());
	if (find == buffer.end())
	{
		this->_state = HEADERS;
		this->_buffer.insert(this->_buffer.end(), buffer.begin(), buffer.end());
		return;
	}
	while(find != buffer.end()){
		line = std::string(buffer.begin(), find);
		if (line.empty())
		{
			buffer.erase(buffer.begin(), find + _crlfPatternSize);
			this->_state = BODY;
			return;
		}
		_httpRequest->addHeader(line);
		buffer.erase(buffer.begin(), find + _crlfPatternSize);
		find = std::search(buffer.begin(), buffer.end(), _crlfPattern.begin(), _crlfPattern.end());
	}
	this->_buffer.insert(this->_buffer.end(), buffer.begin(), buffer.end());
}

void HttpRequestParser::handleBodyState(std::vector<char> &buffer) {
	if (buffer.empty())  
		return;
	if (_httpRequest->isBodyLong())
		writeInFile(buffer);
	else
		writeInMemory(buffer);
	_readBodySize += buffer.size();
	if (_readBodySize >= _MAX_BODY_MEMORY_SIZE && !_httpRequest->isBodyLong()) {
		_httpRequest->setBodyDataFilename(generateUniqueFileName());
		_httpRequest->setBodyLong(true);
		this->_bodyFile.open(_httpRequest->getBodyDataFilename(), std::ios::app);
		std::vector<char> tmpBuffer(this->_httpRequest->getBody().begin(), this->_httpRequest->getBody().end());
		tmpBuffer.insert(tmpBuffer.end(), buffer.begin(), buffer.end());
		this->_httpRequest->getBody().clear();
		writeInFile(tmpBuffer);
	}
	std::multimap<std::string, std::string> &_headers = this->_httpRequest->getHeaders();
	int contentLength = std::atoi(_headers.find("Content-Length")->second.c_str());
	if(_readBodySize >= contentLength)
	{
		this->_state = FINISH;
		this->_bodyFile.close();
	}
}

void HttpRequestParser::writeInMemory(std::vector<char> &buffer) {
	this->_httpRequest->setBody(buffer);
}

void HttpRequestParser::writeInFile(std::vector<char> &buffer) {
	this->_bodyFile.write(buffer.data(), buffer.size());
}

bool HttpRequestParser::isFileExists(const std::string& filename)
{
    std::ifstream file(filename.c_str());
    return file.good();
}


std::string HttpRequestParser::generateUniqueFileName()
{
    std::stringstream filenameStream;
    
    std::time_t currentTime = std::time(NULL);
    filenameStream << "file_" << currentTime << ".txt";
    
    std::string filename = filenameStream.str();
    while (isFileExists(filename))
    {
        currentTime++;
        filenameStream.str("");
        filenameStream << "file_" << currentTime << ".txt";
        filename = filenameStream.str();
    }
    
    return filename;
}

const RequestParseState &HttpRequestParser::getState() {
	return this->_state;
}

const int &HttpRequestParser::getReadBodySize() {
	return this->_readBodySize;
}

const HttpRequest &HttpRequestParser::getHttpRequest() {
	return *(this->_httpRequest);
}

const std::vector<char> &HttpRequestParser::getBuffer() {
	return this->_buffer;
}