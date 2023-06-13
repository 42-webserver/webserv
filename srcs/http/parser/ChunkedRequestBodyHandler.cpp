#include "../../../incs/http/parser/ChunkedRequestBodyHandler.hpp"

ChunkedRequestBodyHandler::ChunkedRequestBodyHandler(void)
: RequestBodyHandler(0)
{}

ChunkedRequestBodyHandler::~ChunkedRequestBodyHandler(void)
{}

bool ChunkedRequestBodyHandler::handleBody(std::vector<char> &reqBuffer, ft::shared_ptr<HttpRequest> req)
{
    if (!this->_buffer.empty())
        reqBuffer.insert(reqBuffer.begin(), this->_buffer.begin(), this->_buffer.end());
    std::vector<char>::iterator find = std::search(reqBuffer.begin(), reqBuffer.end(), _crlfPattern.begin(), _crlfPattern.end());
    while (find != reqBuffer.end()) {
        size_t chunkSize = hexToDec(std::string(reqBuffer.begin(), find));
        if (chunkSize == 0) {
            reqBuffer.clear();
            return true;
        }
        if (reqBuffer.size() >= chunkSize) {
            reqBuffer.erase(reqBuffer.begin(), find + _crlfPattern.size());
            std::vector<char> tmp = std::vector<char>(reqBuffer.begin(), reqBuffer.begin() + chunkSize);
            reqBuffer.erase(reqBuffer.begin(), reqBuffer.begin() + chunkSize + 2);
            if (req->isBodyLong())
                writeInFile(tmp, req);
            else
                writeInMemory(tmp, req);
        }
        else {
            this->_buffer.insert(this->_buffer.end(), reqBuffer.begin(), reqBuffer.end());
            reqBuffer.clear();
            return false;
        }
        find = std::search(reqBuffer.begin(), reqBuffer.end(), _crlfPattern.begin(), _crlfPattern.end());
    }
    this->_buffer.insert(this->_buffer.end(), reqBuffer.begin(), reqBuffer.end());
    reqBuffer.clear();
    return false;
}

void ChunkedRequestBodyHandler::writeInMemory(std::vector<char> &writeBuffer, ft::shared_ptr<HttpRequest> req)
{
    req->insertBody(writeBuffer);
    this->_readBodySize += writeBuffer.size();
    if (this->_readBodySize >= _MAX_BODY_MEMORY_SIZE){
        std::vector<char> tmp = std::vector<char>(req->getBody().begin(), req->getBody().end());
        writeInFile(tmp, req);
        req->setBodyLong(true);
        req->getBody().clear();
    }
}

void ChunkedRequestBodyHandler::writeInFile(std::vector<char> &writeBuffer, ft::shared_ptr<HttpRequest> req)
{
    if (req->getBodyDataFilename().empty())
        req->setBodyDataFilename(FileNameGenerator::generateUniqueFileName());
    std::string fileName = req->getBodyDataFilename();
    std::ofstream file(fileName.c_str(), std::ios::app);
    if (!file.is_open())
        throw std::runtime_error("Error: can't open file");
    file.write(writeBuffer.data(), writeBuffer.size());
    file.close();
}

int ChunkedRequestBodyHandler::hexToDec(const std::string& hexStr) {
    int decValue;
    std::istringstream(hexStr) >> std::hex >> decValue;
    return decValue;
}