#include "../../../incs/http/parser/HttpRequestParser.hpp"

std::string createMultipartRequestBody(const std::string& fieldName, const std::string& fieldValue, const std::string& fileName, const std::string& fileContent) {
    std::string requestBody;
    std::string boundary = "boundary";

    requestBody += "--" + boundary + "\r\n";
    requestBody += "Content-Disposition: form-data; name=\"" + fieldName + "\"; filename=\"" + fileName + "5" + "\"\r\n";
    requestBody += "\r\n";
    requestBody += fieldValue + "\r\n";

    requestBody += "--" + boundary + "\r\n";
    requestBody += "Content-Disposition: form-data; name=\"" + fieldName + "\"; filename=\"" + fileName + "6" + "\"\r\n";
    requestBody += "\r\n";
    requestBody += fileContent + "\r\n";

    requestBody += "--" + boundary + "--\r\n";

    return requestBody;
}

int main(){
	std::string fieldName = "field1";
    std::string fieldValue = "value1lskdajlksajdflkjsdaf";
    std::string fileName = "example1.txt";
    std::string fileContent = "value2psadouf;asdjflsadkjhflksdajflks";

    std::string multipartBodyRequest = createMultipartRequestBody(fieldName, fieldValue, fileName, fileContent);

    std::string requestHeaders = "POST /test.html HTTP/1.1\r\n"
                                 "Host: example.org\r\n"
                                 "Content-Type: multipart/form-data; boundary=\"boundary\"\r\n"
                                 "Content-Length: " + std::to_string(multipartBodyRequest.length()) + "\r\n"
                                 "\r\n";

    std::string completeRequest = requestHeaders + multipartBodyRequest;

	std::vector<char> httpReqeustBuffer;
	httpReqeustBuffer.assign(completeRequest.begin(), completeRequest.end());
	HttpRequestParser httpRequestParser;
	httpRequestParser.parseRequest(httpReqeustBuffer, 1000000);
	ft::shared_ptr <HttpRequest> httpRequest = httpRequestParser.getHttpRequest();
	std::cout << httpRequest->getUri() << "\n" << \
	httpRequest->getProtocol() << std::endl;
	std::cout << httpRequest->getMethod() << std::endl;
	std::cout << httpRequest->getVersion() << std::endl;
	std::cout << httpRequestParser.getState() << std::endl;
	std::cout << httpRequest->isBodyLong() << std::endl;
	std::vector<MultipartRequest> multiparts = httpRequest->getMultipartRequests();

    std::cout << "Multipart requests: " << multiparts.size() << std::endl;
    for(size_t i = 0; i < multiparts.size(); i++){
        std::cout << "part " << i << std::endl;
        if (httpRequest->isBodyLong())
            std::cout << multiparts[i].getBodyDataFilename() << std::endl;
        else
            std::cout << multiparts[i].getMemoryBody().data() << std::endl;
        std::multimap<std::string, std::string> _headers = multiparts[i].getHeaders();
        std::multimap<std::string, std::string>::iterator it;
        for (it = _headers.begin(); it != _headers.end(); it++)
        {
            std::cout << it->first << " : " << it->second << std::endl;
        }
    }

}