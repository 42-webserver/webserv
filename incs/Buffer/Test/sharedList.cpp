#include <iostream>

#include "../../../libs/shared_ptr/shared_ptr.hpp"
#include "../../../libs/Library/Optional.hpp"
#include <list>

void foo() {
    // std::list<ft::shared_ptr<int> > lst;

    // lst.push_back(ft::shared_ptr<int>());
    // lst.push_back(ft::shared_ptr<int>());
    // for (std::list<ft::shared_ptr<int> >::iterator it = lst.begin(); it != lst.end(); it++) {
    //     *it = ft::shared_ptr<int>(new int(10));
    //     std::cout << **it << std::endl;
    // }
    std::list<ft::Optional<ft::shared_ptr<int> > > lst;

    lst.push_back(ft::Optional<ft::shared_ptr<int> >());
    lst.push_back(ft::Optional<ft::shared_ptr<int> >());
    for (std::list<ft::Optional<ft::shared_ptr<int> > >::iterator it = lst.begin(); it != lst.end(); it++) {
        *it = ft::shared_ptr<int>(new int(10));
        std::cout << *it->value() << std::endl;
    }
}
int main(){ 
    ft::Optional<ft::shared_ptr<int> > opt;

    //Alloc test
    opt = ft::shared_ptr<int>(new int(10));

    //shared ptr list test
    foo();
    system("leaks a.out");

}
