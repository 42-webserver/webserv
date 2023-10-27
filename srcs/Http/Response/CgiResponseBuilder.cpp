#include <Http/Response/CgiResponseBuilder.hpp>
#include <Client/Client.hpp>

CgiResponseBuilder::CgiResponseBuilder(std::map<std::string, std::string> &headers,
    ft::shared_ptr<Client> client) : HttpResponseBuilder(client), _headers(headers) {
}
CgiResponseBuilder::~CgiResponseBuilder(void) {
}


void CgiResponseBuilder::_handleStatusCode() {
    if (this->_headers.find("HTTP/1.1") != this->_headers.end()){
        if (this->_headers.find("Location") != this->_headers.end())
            _setStatusCode(FOUND);
        else
            _setStatusCode(OK);
    }
    else {
        _setStatusCode(HttpStatus::stringToHttpStatusCode(this->_headers["HTTP/1.1"]));
    }
}

void CgiResponseBuilder::buildResponseHeader(std::vector<char> &buffer) {
    _handleStatusCode();
    if (this->_headers.find("Content-Type") == this->_headers.end())
        this->_headers["Content-Type"] = "application/octet-stream";

    _allocContentLength(ContentLength::e_content_length_header, 
        std::atol(this->_headers["Content-Length"].c_str()));
    _buildDefaultResponseHeader(buffer);

    static std::set<std::string> defaultHeaders;
    if (defaultHeaders.empty()) {
        defaultHeaders.insert("HTTP/1.1");
        defaultHeaders.insert("Server");
        defaultHeaders.insert("Date");
        defaultHeaders.insert("Connection");
        defaultHeaders.insert("Keep-Alive");
        defaultHeaders.insert("Content-Length");
    }
    for (std::map<std::string, std::string>::iterator it = this->_headers.begin(); it != this->_headers.end(); ++it) {
        if (defaultHeaders.find(it->first) == defaultHeaders.end()) {
            std::string header = it->first + ": " + it->second + "\r\n";
            buffer.insert(buffer.end(), header.begin(), header.end());
        }
    }
    
    // 헤더와 바디 사이에 CRLF 추가
    buffer.insert(buffer.end(), '\r');
    buffer.insert(buffer.end(), '\n');
}