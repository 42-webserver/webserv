#ifndef SHAREDPTR_HPP
# define SHAREDPTR_HPP

# include <stdlib.h>

namespace ft
{

template <typename T>
class shared_ptr {
	private:
		T	*_ptr;
		int	*_count;

	private:
		void	add_ref(void);
		void	release(void);
	
	public:
		shared_ptr(void);
		shared_ptr(T *const ptr);
		shared_ptr(const shared_ptr &ref);
		~shared_ptr(void);
		shared_ptr &operator=(const shared_ptr &ref);
	
	public:
		template <typename U>
		shared_ptr(const shared_ptr<U> &ref);

		template <typename U>
		shared_ptr(const shared_ptr<U> &ref, T *const ptr);

	public:
		T		*get(void) const;
		int		use_count(void) const;
		bool	unique(void) const;
	
	public:
		T			*operator->(void) const;
		T			&operator*(void) const;
		operator	bool(void) const;
	
	public:
		template <typename U>
		bool		operator==(const shared_ptr<U> &ref) const;

		template <typename U>
		bool		operator!=(const shared_ptr<U> &ref) const;
};

template <typename T>
shared_ptr<T>::shared_ptr(void): _ptr(NULL), _count(NULL) {}

template <typename T>
shared_ptr<T>::shared_ptr(T *const ptr): _ptr(ptr), _count(NULL) { this->add_ref(); }

template <typename T>
shared_ptr<T>::shared_ptr(const shared_ptr<T> &ref): _ptr(ref._ptr), _count(ref._count) { this->add_ref(); }

template <typename T>
shared_ptr<T>::~shared_ptr(void) { this->release(); }

template <typename T>
shared_ptr<T> &shared_ptr<T>::operator=(const shared_ptr<T> &ref) {
	if (this != &ref) {
		this->~shared_ptr();
		new (this) shared_ptr<T>(ref);
	}
	return (*this);
}

template <typename T> template <typename U>
shared_ptr<T>::shared_ptr(const shared_ptr<U> &ref): _ptr(ref._ptr), _count(ref._count) { this->add_ref(); }

template <typename T> template <typename U>
shared_ptr<T>::shared_ptr(const shared_ptr<U> &ref, T *const ptr): _ptr(ptr), _count(ref._count) { this->add_ref(); }

template <typename T>
void	shared_ptr<T>::add_ref(void) {
	// Not exist
	if (this->_ptr == NULL) {
		return;
	}

	if (this->_count == NULL) {
		this->_count = new int(1);
		*this->_count = 0;
	}
	++(*this->_count);
}

template <typename T>
void	shared_ptr<T>::release(void) {
	// Not exist
	if (this->_ptr == NULL) {
		return;
	}

	if (--(*this->_count) == 0) {
		delete this->_ptr;
		delete this->_count;
		this->_ptr = NULL;
		this->_count = NULL;
	}
}

template <typename T>
T		*shared_ptr<T>::get(void) const { return (this->_ptr); }

template <typename T>
int		shared_ptr<T>::use_count(void) const { return (*this->_count); }

template <typename T>
bool	shared_ptr<T>::unique(void) const { return (*this->_count == 1); }

template <typename T>
T		*shared_ptr<T>::operator->(void) const { return (this->_ptr); }

template <typename T>
T		&shared_ptr<T>::operator*(void) const { return (*this->_ptr); }

template <typename T>
shared_ptr<T>::operator	bool(void) const { return (this->_ptr != NULL); }

template <typename T> template <typename U>
bool	shared_ptr<T>::operator==(const shared_ptr<U> &ref) const { return (this->_ptr == ref._ptr); }

template <typename T> template <typename U>
bool	shared_ptr<T>::operator!=(const shared_ptr<U> &ref) const { return (this->_ptr != ref._ptr); }

template <typename T>
shared_ptr<T>	make_shared(void) { return (shared_ptr<T>(new T())); }

template <typename T, typename Arg1>
shared_ptr<T>	make_shared(const Arg1 &arg) { return (shared_ptr<T>(new T(const_cast<Arg1 &>(arg)))); }

template <typename T, typename Arg1, typename Arg2>
shared_ptr<T>	make_shared(const Arg1 &arg1, const Arg2 &arg2) { return (shared_ptr<T>(new T(const_cast<Arg1 &>(arg1), const_cast<Arg2 &>(arg2)))); }

template <typename T, typename Arg1, typename Arg2, typename Arg3>
shared_ptr<T>	make_shared(const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3) { return (shared_ptr<T>(new T(const_cast<Arg1 &>(arg1), const_cast<Arg2 &>(arg2), const_cast<Arg3 &>(arg3)))); }

template <typename T, typename U>
shared_ptr<T>	static_pointer_cast(const shared_ptr<U> &ref) { return (shared_ptr<T>(ref, static_cast<T *>(ref.get()))); }

template <typename T, typename U>
shared_ptr<T>	const_pointer_cast(const shared_ptr<U> &ref) { return (shared_ptr<T>(const_cast<T *>(ref.get()))); }

template <typename T, typename U>
shared_ptr<T>	dynamic_pointer_cast(const shared_ptr<U> &ref) {
	T	*ptr = dynamic_cast<T *>(ref.get());
	
	return (ptr ? shared_ptr<T>(ptr) : shared_ptr<T>());
}

template <typename T, typename U>
shared_ptr<T>	reinterpret_pointer_cast(const shared_ptr<U> &ref) { return (shared_ptr<T>(reinterpret_cast<T *>(ref.get()))); }

}

#endif