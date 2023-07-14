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
template <typename T>
unique_ptr<T>	make_unique(void) { return (unique_ptr<T>(new T())); }

template <typename T, typename Arg1>
unique_ptr<T>	make_unique(const Arg1 &arg) { return (unique_ptr<T>(new T(const_cast<Arg1 &>(arg)))); }

template <typename T, typename Arg1, typename Arg2>
unique_ptr<T>	make_unique(const Arg1 &arg1, const Arg2 &arg2) { return (unique_ptr<T>(new T(const_cast<Arg1 &>(arg1), const_cast<Arg2 &>(arg2)))); }

template <typename T, typename Arg1, typename Arg2, typename Arg3>
unique_ptr<T>	make_unique(const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3) { return (unique_ptr<T>(new T(const_cast<Arg1 &>(arg1), const_cast<Arg2 &>(arg2), const_cast<Arg3 &>(arg3)))); }
} // namespace ft

#endif
