#ifndef UNIQUEPTR_HPP
#define UNIQUEPTR_HPP

#include "Assert.hpp"

namespace ft {

template <typename T>
class unique_ptr {
private:
	T* ptr;

public:
	unique_ptr(T* p = 0);
	~unique_ptr();
	
	T& operator*() const;
	T* operator->() const;
	T* get() const;
	T* release();
	void reset(T* p = NULL);
	void swap(unique_ptr& other);

public:
    // using for Move constructor
    unique_ptr(unique_ptr& other) : ptr(other.ptr) {
        other.ptr = NULL;
    }

    // using for Move assignment operator
    unique_ptr& operator=(unique_ptr& other) {
        if (this != &other) {
            delete ptr;
            ptr = other.ptr;
            other.ptr = NULL;
        }
        return *this;
    }
	T* transfer() {
    T* temp = ptr;
    ptr = NULL;
    return temp;
	}

};

template <typename T>
unique_ptr<T>::unique_ptr(T* p) : ptr(p) {}

template <typename T>
unique_ptr<T>::~unique_ptr() {
	delete ptr;
}

template <typename T>
T& unique_ptr<T>::operator*() const {
	ft::Assert::DebugAssert(ptr!= 0, "unique_pointer->set Null");
	return *ptr;
}

template <typename T>
T* unique_ptr<T>::operator->() const {
	ft::Assert::DebugAssert(ptr!= 0, "unique_pointer->set Null");
	return ptr;
}

template <typename T>
T* unique_ptr<T>::get() const {
	ft::Assert::DebugAssert(ptr!= 0, "unique_pointer->set Null");
	return ptr;
}

template <typename T>
T* unique_ptr<T>::release() {
	T* temp = ptr;
	ptr = 0;
	return temp;
}

template <typename T>
void unique_ptr<T>::swap(unique_ptr<T> & other) {
	T * tmp = other.ptr;
	other.ptr = ptr;
	ptr = tmp;
}
template <typename T>
void unique_ptr<T>::reset(T* p) {
	ft::Assert::DebugAssert((p ==0 || p!= ptr),
	"unique_pointer->reset error");
	delete ptr;
	ptr = p;
}
template <typename T>
unique_ptr<T>	make_unique(void) { return (unique_ptr<T>(new T())); }

template <typename T, typename Arg1>
unique_ptr<T>	make_unique(const Arg1 &arg) { return (unique_ptr<T>(new T(const_cast<Arg1 &>(arg)))); }

template <typename T, typename Arg1, typename Arg2>
unique_ptr<T>	make_unique(const Arg1 &arg1, const Arg2 &arg2) { return (unique_ptr<T>(new T(const_cast<Arg1 &>(arg1), const_cast<Arg2 &>(arg2)))); }

template <typename T, typename Arg1, typename Arg2, typename Arg3>
unique_ptr<T>	make_unique(const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3) { return (unique_ptr<T>(new T(const_cast<Arg1 &>(arg1), const_cast<Arg2 &>(arg2), const_cast<Arg3 &>(arg3)))); }

}
#endif
