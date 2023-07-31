#include "../../../incs/Http/Parser/HttpRequestParser.hpp"

std::string createMultipartFormData(const std::string& boundary, const std::string& filename1, const std::string& fieldValue, const std::string& fileName, const std::string& fileContent) {
	std::string formData;
    std::string fieldName = "name1";
	formData += "--" + boundary + "bbbb\r\n";
	formData += "Content-Disposition: form-data; name=\"" + fieldName + "\"; filename=\"" + filename1 + "\"\r\n";
	formData += "\r\n";
	formData += fieldValue + "\r\n";

	formData += "--" + boundary + "aaaa\r\n";
	formData += "Content-Disposition: form-data; name=\"" + fieldName + "\"; filename=\"" + fileName + "\"\r\n";
	formData += "Content-Type: application/octet-stream\r\n";
	formData += "\r\n";
	formData += fileContent + "\r\n";

	formData += "--" + boundary + "--\r\n";

	return formData;
}

std::string createMultipartRequestBody(const std::string& filename1, const std::string& fieldValue, const std::string& fileName, const std::string& fileContent) {
	std::string requestBody;
	std::string boundary = "----Boundary" + std::to_string(std::rand());
	requestBody += "POST / Http/1.1\r\n";
	requestBody += "Host: localhost:8080\r\n";
	requestBody += "User-Agent: curl/7.64.1\r\n";
	requestBody += "Content-Type: multipart/form-data; boundary=" + boundary + "\r\n";
	requestBody += "Content-Length: " + std::to_string(createMultipartFormData(boundary, filename1, fieldValue, fileName, fileContent).length()) + "\r\n";
	requestBody += "\r\n";
	requestBody += createMultipartFormData(boundary, filename1, fieldValue, fileName, fileContent);

	return requestBody;
}

int main(){
	std::string filename1 = "example7.txt";
	std::string fieldValue = "This is a text field value.";
	std::string fileName = "example8.txt";
	std::string fileContent = "This is the content of the file.";

	 std::string multipartBodyRequest = createMultipartRequestBody(filename1, fieldValue, fileName, fileContent);

	std::vector<char> httpReqeustBuffer;
	httpReqeustBuffer.assign(multipartBodyRequest.begin(), multipartBodyRequest.end());
	HttpRequestParser httpRequestParser;
	httpRequestParser.parseRequest(httpReqeustBuffer, 1000000);
	ft::shared_ptr <HttpRequest> httpRequest = httpRequestParser.getHttpRequest();
	std::cout << httpRequest->getUri() << "\n" << \
	httpRequest->getProtocol() << std::endl;
	std::cout << httpRequest->getMethod() << std::endl;
	std::cout << httpRequest->getVersion() << std::endl;
	std::cout << httpRequestParser.getState() << std::endl;
}