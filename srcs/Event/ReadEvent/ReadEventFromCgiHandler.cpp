#include <Event/ReadEvent/ReadEventFromCgiHandler.hpp>
#include <Buffer/Node/NormalNode.hpp>
#include <Event/ReadEvent/ReadEventFromCgi.hpp>
#include <Http/Parser/ParsePatternMatcher.hpp>
ReadEventFromCgiHandler::ReadEventFromCgiHandler()
: _buffer(NormalNode::_sizeNormal), _state(e_parse_cgi_start),
_realSize(0), _firstInCgiBody(true), _capacity(NormalNode::_sizeNormal) {}
ReadEventFromCgiHandler::~ReadEventFromCgiHandler(){}


std::string ReadEventFromCgiHandler::_spaceTrim(const std::string& str) {
    size_t start = str.find_first_not_of(' ');
    size_t end = str.find_last_not_of(' ');
    if (start == std::string::npos || end == std::string::npos) {
        return "";
    }
    return str.substr(start, end - start + 1);
}

bool ReadEventFromCgiHandler::_checkStartBody(std::vector<char>::iterator find, std::vector<char>::iterator end) {
   return (find + 2 * _crlfPattern.size() <= end && 
        *(find + _crlfPattern.size()) == '\r' && 
        *(find + _crlfPattern.size() + 1) == '\n');
}



void ReadEventFromCgiHandler::_addHeader(const std::string& key, const std::string& value) {
    this->_headers[key] = value;
}


std::vector<char>::iterator ReadEventFromCgiHandler::_begin() {
    return this->_buffer.begin();
}

std::vector<char>::iterator ReadEventFromCgiHandler::_end() {
    return this->_begin() + this->_realSize;
}

void ReadEventFromCgiHandler::_parseToBody(void){
    std::cerr << "ReadEventFromCgiHandler::_parseToBody" << std::endl;
    std::vector<char>& buffer = this->_buffer;
    std::vector<char>::iterator current = _begin();
    std::vector<char>::iterator end = _end();
    std::vector<std::string> headers;
    std::string line;
    int distance = std::distance(current, end);
    std::cerr << "distance:" << distance << std::endl;
    for (int i = 0; i < distance; i++)
        std::cerr << buffer[i];
    while (current != end) {
        std::vector<char>::iterator find = std::search(current, end, _crlfPattern.begin(), _crlfPattern.end());
        if (find == end) {
            std::cerr << "ReadEventFromCgiHandler::_parseToBody->find == end" << std::endl;
            break ;
        }

        line = std::string(current, find);
        current = find + _crlfPattern.size();
        if (line.empty()) {
            break ;
        }
        headers.push_back(line);
    }
    
    for (size_t i = 0; i < headers.size(); ++i) {
        const std::string& header = headers[i];
        size_t separator = header.find(':'); // ':' 기준으로 헤더 파싱
        if (separator != std::string::npos) {
            std::string key = header.substr(0, separator);
            std::string value = header.substr(separator + 1);

            // 공백 제거
            key = _spaceTrim(key);
            value = _spaceTrim(value);
            _addHeader(key, value);
        }    
    }
    _realSize = std::distance(current, end);

    if (line.empty()) {
        this->_state = e_parse_cgi_body;
        buffer.erase(buffer.begin(), current);
    }
    else {
        this->_state = e_parse_cgi_header;
    }
}

void ReadEventFromCgiHandler::_handleCgiStart(ReadEventFromCgi& event){
    std::cerr << "ReadEventFromCgiHandler::_handleCgiStart" << std::endl;
    std::vector<char>& buffer = this->_buffer;
    ft::shared_ptr<Client> client = event.getClient();
    ft::shared_ptr<HttpResponse> response = client->getResponse();
    ssize_t readSize = read(event.getFd(), buffer.data(), _capacity);
    
    if (readSize <= 0){
        std::cerr << "ReadEventFromCgiHandler::_handleCgiStart->readSize <= 0" << std::endl;
        _callErrorPageBuilder(event);
        event.offboardQueue();
        return ;
    }
    std::cerr << "ReadEventFromCgiHandler::_handleCgiStart->readSize > 0" << std::endl;
    for (int i = 0; i < readSize; i++)
        std::cerr << buffer[i];
    std::cerr << std::endl;
    response->setCgiSync(e_cgi_reading);
    this->_realSize = readSize;
    _parseToBody();
}

void ReadEventFromCgiHandler::_callErrorPageBuilder(ReadEventFromCgi &readEventFromCgi) {
    ft::shared_ptr<Client> client = readEventFromCgi.getClient();
    ft::shared_ptr<ErrorPageBuilder> errorPageBuilder(new ErrorPageBuilder(client, BAD_GATEWAY));
    errorPageBuilder->buildResponseHeader(client->getResponse()
    ->getNormalCaseBuffer(HttpResponse::AccessKey()));
    client->getResponse()->setCgiSync(e_cgi_reading_done);
    this->_state = e_parse_cgi_end;
}
void ReadEventFromCgiHandler::_handleCgiHeader(ReadEventFromCgi& event){
    std::cerr << "ReadEventFromCgiHandler::_handleCgiHeader" << std::endl;
    std::vector<char>& buffer = this->_buffer;
    ft::shared_ptr<Client> client = event.getClient();
    size_t readSize = read(event.getFd(), buffer.data() + this->_realSize, 
    _capacity - this->_realSize);

    if (readSize <= 0){
        _callErrorPageBuilder(event);
            event.offboardQueue();
        return ;
    }
    this->_realSize += readSize;
    _parseToBody();
}

void ReadEventFromCgiHandler::_handleCgiBody(ReadEventFromCgi& event){
    std::cerr << "ReadEventFromCgiHandler::_handleCgiBody" << std::endl;
    ft::shared_ptr<HttpResponse> response = event.getClient()->getResponse();

    if (_firstInCgiBody) {
        std::cerr << "ReadEventFromCgiHandler::_handleCgiBody->_firstInCgiBody" << std::endl;
        ft::shared_ptr<Client> client = event.getClient();
        response->setResponseSize(BigSize, HttpResponse::AccessKey());
        response->allocateBigSizeBuffer(HttpResponse::AccessKey());

        bool isContentLengthSet = _headers.find("Content-Length") != _headers.end();
        if (isContentLengthSet)
            _contentLength = std::atoi(_headers["Content-Length"].c_str());
        else
            _contentLength = -1;
        ft::shared_ptr<IoReadAndWriteBuffer> bigSizeBuffer = response->getBigSizeBuffer(HttpResponse::AccessKey());
        bigSizeBuffer->append(_begin(), _end());
        std::cerr << "bigSizeBuffer->size():" << bigSizeBuffer->size() << std::endl;
        _firstInCgiBody = false;
    }
    ft::shared_ptr<IoReadAndWriteBuffer> bigSizeBuffer = response->getBigSizeBuffer(HttpResponse::AccessKey());

    if (_contentLength > 0) {
        std::cerr << "ReadEventFromCgiHandler::_handleCgiBody->contentLength > 0" << std::endl;
        // _previousReadSize += bigSizeBuffer->ioRead(event.getFd(), contentLength - _previousReadSize);
        ssize_t n = bigSizeBuffer->ioReadToRemainigSize(event.getFd(), _contentLength - _previousReadSize);
        if (n == 0) {
            this->_state = e_parse_cgi_end;
            _contentLength = _previousReadSize;
            _handleCgiEnd(event);
            return ;
        }
        if (n < 0) {
            std::cerr << "ReadEventFromCgiHandler::_handleCgiBody->contentLength > 0->n < 0" << std::endl;
            _callErrorPageBuilder(event);
            event.offboardQueue();
            return ;
        }
        _previousReadSize += n;
        if (_previousReadSize == _contentLength) {
            this->_state = e_parse_cgi_end;
        }
    }
    else {
        std::cerr << "ReadEventFromCgiHandler::_handleCgiBody->contentLength <= 0" << std::endl;
        int fd = event.getFd();
        std::cerr << "fd:" << fd << std::endl;
        std::cerr << "bigSizeBuffer->size():" << bigSizeBuffer->size() << std::endl;
        size_t n = bigSizeBuffer->ioRead(fd);
        std::cerr << "n:" << n << std::endl;
        if (n < 0) {
            std::cerr << "ReadEventFromCgiHandler::_handleCgiBody->contentLength <= 0->n < 0" << std::endl;
            _callErrorPageBuilder(event);
            event.offboardQueue();
            return ;
        }
        if (n == 0) {
            std::cerr << "ReadEventFromCgiHandler::_handleCgiBody->contentLength <= 0->n == 0" << std::endl;
            this->_state = e_parse_cgi_end;
        }
    }
    if (this->_state == e_parse_cgi_end)
        _handleCgiEnd(event);
}

void ReadEventFromCgiHandler::_buildCgiResponseHeader(ReadEventFromCgi& event){
    std::cerr << "ReadEventFromCgiHandler::_buildCgiResponseHeader" << std::endl;
    ft::shared_ptr<Client> client = event.getClient();
    std::vector<char>& buffer = client->getResponse()->getNormalCaseBuffer(HttpResponse::AccessKey());
    ft::shared_ptr<HttpResponseBuilder> builder(new CgiResponseBuilder(this->_headers, client));
    builder->buildResponseHeader(buffer);

}   

void ReadEventFromCgiHandler::_handleCgiEnd(ReadEventFromCgi& event){
    std::cerr << "ReadEventFromCgiHandler::_handleCgiEnd" << std::endl;
    ft::shared_ptr<Client> client = event.getClient();
    ft::shared_ptr<HttpResponse> response = client->getResponse();
    ft::shared_ptr<IoReadAndWriteBuffer> bigSizeBuffer = response->getBigSizeBuffer(HttpResponse::AccessKey());
    if (_contentLength < 0) {
        _headers["Content-Length"] = std::to_string(bigSizeBuffer->size());
    }

    _buildCgiResponseHeader(event);
    response->setCgiSync(e_cgi_reading_done);
    std::cerr << "ReadEventFromCgiHandler::_handleCgiEnd- normal" << std::endl;
    std::vector<char> normalSizeBuffer = response->getNormalCaseBuffer(HttpResponse::AccessKey());
    for (size_t i = 0; i < normalSizeBuffer.size(); i++)
        std::cerr << normalSizeBuffer[i];
    std::cerr << std::endl;
    std::cerr << "ReadEventFromCgiHandler::_handleCgiEnd- big" << std::endl;
    bigSizeBuffer->printBuffer();
    event.offboardQueue();
}
void ReadEventFromCgiHandler::handleEvent(Event &event){
    std::cerr << "ReadEventFromCgiHandler::handleEvent" << std::endl;
   ReadEventFromCgi *readEventFromCgi = static_cast<ReadEventFromCgi*>(&event);
    std::cerr << "this state:" << this->_state << std::endl;
    switch (this->_state){
        case e_parse_cgi_start:
                this->_handleCgiStart(*readEventFromCgi);
                break;
         case e_parse_cgi_header:
              this->_handleCgiHeader(*readEventFromCgi);
              break;
         case e_parse_cgi_body:
              this->_handleCgiBody(*readEventFromCgi);
              break;
         case e_parse_cgi_end:
              throw std::runtime_error("ReadEventFromCgiHandler::_handleCgiEnd->logic error");
              break;
         default:
              break;
    }

}

std::map<std::string, std::string> &ReadEventFromCgiHandler::getHeaders(){
    return this->_headers;
}