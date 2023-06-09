#ifndef UNIQUEPTR_HPP
#define UNIQUEPTR_HPP

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
	void reset(T* p = 0);

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
	return *ptr;
}

template <typename T>
T* unique_ptr<T>::operator->() const {
	return ptr;
}

template <typename T>
T* unique_ptr<T>::get() const {
	return ptr;
}

template <typename T>
T* unique_ptr<T>::release() {
	T* temp = ptr;
	ptr = 0;
	return temp;
}

template <typename T>
void unique_ptr<T>::reset(T* p) {
	if (ptr != p) {
		delete ptr;
		ptr = p;
	}
}

} // namespace ft

#endif
