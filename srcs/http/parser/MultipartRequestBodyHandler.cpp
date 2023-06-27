#include "../../../incs/http/parser/MultipartRequestBodyHandler.hpp"

MultipartRequestBodyHandler::MultipartRequestBodyHandler(std::string boundary)
    : RequestBodyHandler(0), _targetIdx(0), _state(M_HEADER)
{
    boundary = std::string(boundary.begin() + 1, boundary.end() - 1);
    this->_boundaryStart = "--" + boundary;
    this->_boundaryEnd =  "--" + boundary + "--";
}

MultipartRequestBodyHandler::~MultipartRequestBodyHandler(void)
{
}

bool MultipartRequestBodyHandler::handleBody(std::vector<char> &reqBuffer, ft::shared_ptr<HttpRequest> req)
{
    if (!this->_buffer.empty()) {
		reqBuffer.insert(reqBuffer.begin(), this->_buffer.begin(), this->_buffer.end());
		this->_buffer.clear();
	}
    bool result = false;
    while (!reqBuffer.empty()){
        if (this->_state == M_HEADER)
            handleMultipartHeader(reqBuffer, req);
        if (this->_state == M_BODY)
            result = parsePartOfBody(reqBuffer, req);
    }
    if(result)
        FileUploader::fileUpload(req->getMultipartRequests());
    return result;
}

void MultipartRequestBodyHandler::handleMultipartHeader(std::vector<char> &reqBuffer, ft::shared_ptr<HttpRequest> req){
    try{
        req->getMultipartRequests().at(_targetIdx);
    } catch (std::out_of_range &e){
        req->getMultipartRequests().push_back(MultipartRequest());
    }

    std::vector<char>::iterator find = std::search(reqBuffer.begin(), reqBuffer.end(), _crlfPattern.begin(), _crlfPattern.end());
    std::string line;
    while (find != reqBuffer.end()){
        MultipartRequest & tarMultipart = req->getMultipartRequests().at(_targetIdx);
        line = std::string(reqBuffer.begin(), find);
        reqBuffer.erase(reqBuffer.begin(), find + _crlfPatternSize);
        if (line == _boundaryStart){
            find = std::search(reqBuffer.begin(), reqBuffer.end(), _crlfPattern.begin(), _crlfPattern.end());
            continue;
        }
        if (line.empty()){
            this->_state = M_BODY;
            return;
        }
        tarMultipart.addHeader(line);
        find = std::search(reqBuffer.begin(), reqBuffer.end(), _crlfPattern.begin(), _crlfPattern.end());
    }
    this->_buffer.insert(this->_buffer.end(), reqBuffer.begin(), reqBuffer.end());
    reqBuffer.clear();
}

bool MultipartRequestBodyHandler::parsePartOfBody(std::vector<char> &reqBuffer, ft::shared_ptr<HttpRequest> req){
    std::vector<char>::iterator find = std::search(reqBuffer.begin(), reqBuffer.end(), _crlfPattern.begin(), _crlfPattern.end());
    if (find != reqBuffer.end()){
        std::vector<char> part(reqBuffer.begin(), find);
        reqBuffer.erase(reqBuffer.begin(), find + _crlfPatternSize);
        writeParts(part, req);
        _targetIdx++;
        this->_state = M_HEADER;
    }else{
        writeParts(reqBuffer, req);
        reqBuffer.clear();
    }
    find = std::search(reqBuffer.begin(), reqBuffer.end(), _crlfPattern.begin(), _crlfPattern.end());
    if (find != reqBuffer.end() && std::string(reqBuffer.begin(), find) == _boundaryEnd){
        reqBuffer.clear();
        return true;
    }
    return false;
}

void MultipartRequestBodyHandler::writeParts(std::vector<char> &reqBuffer, ft::shared_ptr<HttpRequest> req){
    if (req->isBodyLong())
        writeInFile(reqBuffer, req);
    else
        writeInMemory(reqBuffer, req);
}

void MultipartRequestBodyHandler::writeInFile(std::vector<char> &reqBuffer, ft::shared_ptr<HttpRequest> req){
    MultipartRequest &multipartRequest = req->getMultipartRequests().at(_targetIdx);
    if (multipartRequest.getBodyDataFilename().empty())
        multipartRequest.setBodyDataFilename(FileNameGenerator::generateUniqueFileName());
    std::string fileName = multipartRequest.getBodyDataFilename();
    std::ifstream file2(fileName.c_str());
    std::ofstream file(fileName.c_str(), std::ios::app);
    if (!file.is_open())
        throw std::runtime_error("Error: can't open file");
    file.write(reqBuffer.data(), reqBuffer.size());
    file.close();
    this->_readBodySize += reqBuffer.size();
}

void MultipartRequestBodyHandler::writeInMemory(std::vector<char> &reqBuffer, ft::shared_ptr<HttpRequest> req){
    MultipartRequest &multipartRequest = req->getMultipartRequests().at(_targetIdx);
    multipartRequest.insertBody(reqBuffer);
    this->_readBodySize += reqBuffer.size();
}