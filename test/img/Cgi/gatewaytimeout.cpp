#include <iostream>
#include <unistd.h>
int main(void){
    while (1) {
        std::cout << "Content-type: text/html\r\n\r\n";
        sleep(10000);
    }
}