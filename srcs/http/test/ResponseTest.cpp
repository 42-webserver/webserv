#include "../../../incs/http/parser/HttpParser.hpp"
#include "../../../incs/http/request/HttpRequest.hpp"
#include "../../../incs/http/response/HttpResponse.hpp"
#include <iostream>

int main(){
    HttpResponse res;
    res.setBody(new std::string("body data!!!!!!!!!!!!!!!!!!!!!!!!!!"));
    res.addHeader("Content-Type", "text/html");
    res.addHeader("Content-Length", "100");
    res.addHeader("Connection", "keep-alive");
    res.addHeader("Set-Cookie", "cookie1=cookie1value");
    res.setStatusCode(OK);

    HttpParser parser;
    std::cout << *parser.parseResponse(&res) << std::endl;
}