#include <iostream>
#include <vector>

class a{
private:
    std::vector<int> v;
public:
    template <typename RandomAccessIterator>
    void insert(RandomAccessIterator start, RandomAccessIterator end){
        v.insert(v.end(), start, end);
    };
};
int main(void){
    std::vector<int> v;
}