#ifndef SINGLETON_HPP
# define SINGLETON_HPP

#include <iostream>
#include "../unique_ptr/unique_ptr.hpp"

template <typename T>
class Singleton {
public:
    static T& getInstance() {
        if (instance.get() == NULL) {
            instance.reset(new T());
        }
        return *instance.get();
    }

protected:
    Singleton() {}

private:
    Singleton(const Singleton&);
    Singleton& operator=(const Singleton&);

    static ft::unique_ptr<T> instance;
};

#endif