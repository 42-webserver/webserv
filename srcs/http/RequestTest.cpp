#include "parser/HttpParser.hpp"
#include "request/HttpRequest.hpp"
#include "response/HttpResponse.hpp"
#include <iostream>

int main(){
    std::string request = "GET / HTTP/1.1\r\n"
                          "Host: localhost:8080\r\n"
                          "Connection: keep-alive\r\n"
                          "Cache-Control: max-age=0\r\n"
                          "Upgrade-Insecure-Requests: 1\r\n"
                          "User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/51.0.2704.103 Safari/537.36\r\n"
                          "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n"
                          "Accept-Encoding: gzip, deflate, sdch\r\n"
                          "Accept-Language: en-US,en;q=0.8,fr;q=0.6\r\n"
                          "Cookie: _ga=GA1.1.1111111111.1111111111\r\n"
                          "\r\n"
                          "body data!!!!!!!!!!!!!!!!!!!!!!!!!!";
    HttpParser parser;
    HttpRequest req = parser.parseRequest(request);
    std::cout << "method = " << req.getMethod() << std::endl;
    std::cout << "uri = " << req.getUri() << std::endl;
    std::cout << "version = " << req.getVersion() << std::endl;
    std::cout << "protocol = " << req.getProtocol() << std::endl;
    std::cout << "body = " << req.getBody() << std::endl;
    std::multimap<std::string, std::string> headers = req.getHeaders();
    std::map<std::string, std::string> cookies = req.getCookies();
    std::map<std::string, std::string> queries = req.getQueries();
    std::cout << "====== headers ======" << std::endl;
    for (std::multimap<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); it++)
        std::cout << it->first << ": " << it->second << std::endl;
    std::cout << "====== cookies ======" << std::endl;           
    for (std::map<std::string, std::string>::iterator it = cookies.begin(); it != cookies.end(); it++)
        std::cout << it->first << ": " << it->second << std::endl;
    std::cout << "====== queries ======" << std::endl;
    for (std::map<std::string, std::string>::iterator it = queries.begin(); it != queries.end(); it++)
        std::cout << it->first << ": " << it->second << std::endl;
    
    return 0;
}