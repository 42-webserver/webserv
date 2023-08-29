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

private:
	unique_ptr(const unique_ptr&);
	unique_ptr& operator=(const unique_ptr&);
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
}
#endif
