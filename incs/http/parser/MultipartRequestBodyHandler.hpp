#ifndef MULTIPART_REQUEST_BODY_HANDELR_HPP
#define MULTIPART_REQUEST_BODY_HANDELR_HPP

#include "RequestBodyHandler.hpp"
#include <algorithm>
#include "ParsePatternMatcher.hpp"
#include "../Utils/FileUploader.hpp"

typedef enum MultipartState{
    M_HEADER,
    M_BODY
} MultipartParseState;

class MultipartRequestBodyHandler : public RequestBodyHandler {
private:
    std::vector<char>           _buffer;
    std::string                 _boundaryStart;
    std::string                 _boundaryEnd;
    int                         _targetIdx;
    MultipartParseState         _state;

public:
    bool handleBody(std::vector<char> &reqBuffer, ft::shared_ptr<HttpRequest> req);
    MultipartRequestBodyHandler(std::string boundary);
    ~MultipartRequestBodyHandler(void);


private:
    void handleMultipartHeader(std::vector<char> &reqBuffer, ft::shared_ptr<HttpRequest> req);
    bool parsePartOfBody(std::vector<char> &reqBuffer, ft::shared_ptr<HttpRequest> req);
    void writeInFile(std::vector<char> &reqBuffer, ft::shared_ptr<HttpRequest> req);
    void writeInMemory(std::vector<char> &reqBuffer, ft::shared_ptr<HttpRequest> req);
    void writeParts(std::vector<char> &reqBuffer, ft::shared_ptr<HttpRequest> req);
    
};

#endif