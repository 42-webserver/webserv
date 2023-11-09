#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

int main(void) {
    std::fstream f;
    f.open("test.txt", std::ios::out);

    // 입력 받은 내용을 저장할 문자열
    std::string inputContent;
    std::string line;
    
    // 표준 입력에서 한 줄씩 읽어 저장
    while (std::getline(std::cin, line)) {
        inputContent += line + "<br>\n"; // HTML 줄바꿈을 위해 <br> 태그 추가
    }

    // HTML 문서 생성
    std::ostringstream oss;
    oss << "Content-type: text/html\r\n\r\n"; // HTTP 헤더
    oss << "<html>\n";
    oss << "<head>\n";
    oss << "<title>CGI Program</title>\n";
    oss << "</head>\n";
    oss << "<body>\n";
    oss << "<h1>This is my first CGI program</h1>\n";
    oss << "<p>Received input:</p>\n";
    oss << "<pre>" << inputContent << "</pre>\n"; // pre 태그를 사용하여 형식을 그대로 유지
    oss << "</body>\n";
    oss << "</html>\n";

    // 표준 출력으로 HTML 문서 내보내기
    std::cout << oss.str();

    // 파일에도 동일한 내용 기록
    f << oss.str();
    f.flush();
    f.close();

    return 0;
}
