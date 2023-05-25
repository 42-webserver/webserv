#include <iostream>
#include <vector>
#include "../../incs/cache/cache.hpp"


int main() {
    LruCache cache(5); // 캐시 크기는 5로 설정

    // 데이터 추가
    cache.put("uri1", {'A', 'B', 'C'});
    cache.put("uri2", {'D', 'E', 'F'});
    cache.put("uri3", {'G', 'H', 'I'});
    cache.put("uri4", {'J', 'K', 'L'});
    cache.put("uri5", {'M', 'N', 'O'});

    // 캐시에서 데이터 가져오기
    std::vector<char> data1 = cache.get("uri1"); // 존재하는 데이터
    std::vector<char> data2 = cache.get("uri3"); // 존재하는 데이터
    std::vector<char> data3 = cache.get("uri6"); // 캐시에 존재하지 않는 데이터

    // 데이터 출력
    if (!data1.empty()) {
        std::cout << "Data1: ";
        for (std::vector<char>::iterator it = data1.begin(); it != data1.end(); ++it) {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
    } else {
        std::cout << "Data1 not found in cache." << std::endl;
    }
    
    if (!data2.empty()) {
        std::cout << "Data2: ";
        for (std::vector<char>::iterator it = data2.begin(); it != data2.end(); ++it) {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
    } else {
        std::cout << "Data2 not found in cache." << std::endl;
    }
    
    if (!data3.empty()) {
        std::cout << "Data3: ";
        for (std::vector<char>::iterator it = data3.begin(); it != data3.end(); ++it) {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
    } else {
        std::cout << "Data3 not found in cache." << std::endl;
    }

    // 새로운 데이터 추가 후 반복적인 테스트
    for (int i = 1; i <= 5; ++i) {
        std::string uri = "new_uri" + std::to_string(i);
        std::vector<char> content = {static_cast<char>('A' + i)};
        cache.put(uri, content);

        std::cout << "Data" << i + 3 << ": ";
        std::vector<char> data = cache.get(uri);
        if (!data.empty()) {
            for (std::vector<char>::iterator it = data.begin(); it != data.end(); ++it) {
                std::cout << *it << " ";
            }
        } else {
            std::cout << "Data not found in cache.";
        }
        std::cout << std::endl;
    }

    return 0;
}
