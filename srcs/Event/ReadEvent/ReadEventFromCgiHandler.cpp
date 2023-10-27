#include <Event/ReadEvent/ReadEventFromCgiHandler.hpp>
#include <Buffer/Node/NormalNode.hpp>
#include <Event/ReadEvent/ReadEventFromCgi.hpp>
#include <Http/Parser/ParsePatternMatcher.hpp>
ReadEventFromCgiHandler::ReadEventFromCgiHandler()
: _buffer(NormalNode::_sizeNormal), _state(e_parse_cgi_header),
_realSize(0){}
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

void ReadEventFromCgiHandler::_parseToBody(ReadEventFromCgi& event){
    std::vector<char> buffer = this->_buffer;
    std::vector<char>::iterator current = _begin();
    std::vector<char>::iterator end = _end();

    while (current != end) {
        std::vector<char>::iterator find = std::search(current, end, _crlfPattern.begin(), _crlfPattern.end());
        if (find == end) {
            this->_state = e_parse_cgi_header;
            return ;
        }

        if (this->_checkStartBody(find, end)) {
            std::vector<char>::iterator bodyStartIt = find + 2 * _crlfPattern.size();
            size_t bodyLength = end - bodyStartIt;

            // memmove를 사용하여 바디 내용을 버퍼의 시작 부분으로 이동
            std::memmove(&buffer[0], &(*bodyStartIt), bodyLength);

            // 실제 버퍼 내용 길이를 갱신
            this->_realSize = bodyLength;

            this->_state = e_parse_cgi_body;
            _handleCgiBody(event);
            return;
        }


        std::string line(current, find);

        size_t separator = line.find(':'); // ':' 기준으로 헤더 파싱
        if (separator != std::string::npos) {
            std::string key = line.substr(0, separator);
            std::string value = line.substr(separator + 1);

            // 공백 제거
            key = _spaceTrim(key);
            value = _spaceTrim(value);
            _addHeader(key, value);
        }

        current = find + _crlfPattern.size();
    }
    size_t remaining = end - current;
    if (remaining > 0) {
        std::memmove(&buffer[0], &(*current), remaining);
    }
    this->_realSize = remaining;
}

void ReadEventFromCgiHandler::_handleCgiStart(ReadEventFromCgi& event){
    std::vector<char> buffer = this->_buffer;
    ft::shared_ptr<Client> client = event.getClient();
    ft::shared_ptr<HttpResponse> response = client->getResponse();
    size_t readSize = read(event.getFd(), &buffer[0], buffer.size());
    if (readSize == 0){
        ft::shared_ptr<ErrorPageBuilder> errorPageBuilder(new ErrorPageBuilder(client, BAD_GATEWAY));
        errorPageBuilder->buildResponseHeader(response
        ->getNormalCaseBuffer(HttpResponse::AccessKey()));
        return ;
    }
 
    response->setCgiSync(e_cgi_reading);
    this->_realSize = readSize;
    _parseToBody(event);
}

void ReadEventFromCgiHandler::_handleCgiHeader(ReadEventFromCgi& event){
    std::vector<char> buffer = this->_buffer;
    ft::shared_ptr<Client> client = event.getClient();
    size_t readSize = read(event.getFd(), &buffer[0] + this->_realSize, buffer.size() - this->_realSize);

    if (readSize == 0){
        ft::shared_ptr<ErrorPageBuilder> errorPageBuilder(new ErrorPageBuilder(client, BAD_GATEWAY));
        errorPageBuilder->buildResponseHeader(client->getResponse()
        ->getNormalCaseBuffer(HttpResponse::AccessKey()));
        return ;
    }
    this->_realSize += readSize;
    _parseToBody(event);
}

void ReadEventFromCgiHandler::_handleCgiBody(ReadEventFromCgi& event){
    ft::shared_ptr<HttpResponse> response = event.getClient()->getResponse();
    ft::shared_ptr<IoReadAndWriteBuffer> bigSizeBuffer = response->getBigSizeBuffer(HttpResponse::AccessKey());
    if (_firstInCgiBody) {
        ft::shared_ptr<Client> client = event.getClient();
        response->setResponseSize(BigSize, HttpResponse::AccessKey());

        this->_isContentLengthSet = _headers.find("Content-Length") != _headers.end();
    }
    if (_isContentLengthSet) {
        size_t contentLength = std::atoi(_headers["Content-Length"].c_str()) - this->_realSize;
        // _previousReadSize += bigSizeBuffer->ioRead(event.getFd(), contentLength - _previousReadSize);
        _previousReadSize += bigSizeBuffer->ioReadToRemainigSize(event.getFd(), contentLength - _previousReadSize);
        if (_previousReadSize == contentLength) {
            this->_state = e_parse_cgi_end;
        }
    }
    else {
        size_t n = bigSizeBuffer->ioRead(event.getFd());
        if (n == 0) {
            this->_state = e_parse_cgi_end;
        }
    }
    if (this->_state == e_parse_cgi_end)
        _handleCgiEnd(event);
}

void ReadEventFromCgiHandler::_buildCgiResponseHeader(ReadEventFromCgi& event){
    ft::shared_ptr<Client> client = event.getClient();
    std::vector<char> buffer = client->getResponse()->getNormalCaseBuffer(HttpResponse::AccessKey());
    ft::shared_ptr<HttpResponseBuilder> builder(new CgiResponseBuilder(this->_headers, client));
    builder->buildResponseHeader(buffer);

    buffer.insert(buffer.end(), this->_begin(), this->_end());

}   

void ReadEventFromCgiHandler::_handleCgiEnd(ReadEventFromCgi& event){
    ft::shared_ptr<Client> client = event.getClient();
    ft::shared_ptr<HttpResponse> response = client->getResponse();
    ft::shared_ptr<IoReadAndWriteBuffer> bigSizeBuffer = response->getBigSizeBuffer(HttpResponse::AccessKey());
    if (!_isContentLengthSet)
        _headers["Content-Length"] = bigSizeBuffer->size() + _realSize;

    
    _buildCgiResponseHeader(event);
    response->setCgiSync(e_cgi_reading_done);
    event.offboardQueue();
}
void ReadEventFromCgiHandler::handleEvent(Event &event){
   ReadEventFromCgi *readEventFromCgi = static_cast<ReadEventFromCgi*>(&event);

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