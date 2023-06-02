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
		shared_ptr(T *ptr);
		shared_ptr(const shared_ptr<T> &ref);
		~shared_ptr(void);
		shared_ptr<T> &operator=(const shared_ptr<T> &ref);

	public:
		T		*get(void) const;
		int		use_count(void) const;
		bool	unique(void) const;
	
	public:
		T			*operator->(void) const;
		T			&operator*(void) const;
		bool		operator==(const shared_ptr<T> &ref) const;
		bool		operator!=(const shared_ptr<T> &ref) const;
		operator	bool(void) const;
};

template <typename T>
shared_ptr<T>::shared_ptr(void): _ptr(NULL), _count(NULL) {}

template <typename T>
shared_ptr<T>::shared_ptr(T *ptr): _ptr(ptr), _count(NULL) { this->add_ref(); }

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
bool	shared_ptr<T>::operator==(const shared_ptr<T> &ref) const { return (this->_ptr == ref._ptr); }

template <typename T>
bool	shared_ptr<T>::operator!=(const shared_ptr<T> &ref) const { return (this->_ptr != ref._ptr); }

template <typename T>
shared_ptr<T>::operator	bool(void) const { return (this->_ptr != NULL); }

//잘못된 구현
// template <typename T>
// shared_ptr<T>	make_shared(T arg) { return (shared_ptr<T>(new T(arg))); }

}

#endif