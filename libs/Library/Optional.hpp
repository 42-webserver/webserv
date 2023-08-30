#ifndef OPTIONAL_HPP
# define OPTIONAL_HPP

#include <stdexcept>
#include <cstring>

namespace ft {

    struct nullopt_t {
       	nullopt_t() {}
    };
    const nullopt_t nullopt;

    template<typename T>
    class Optional {
    private:
        char _storage[sizeof(T)] __attribute__((aligned(__alignof__(T))));
        bool _hasValue;

        T* pointer() {
            return reinterpret_cast<T*>(_storage);
        }

        const T* pointer() const {
            return reinterpret_cast<const T*>(_storage);
        }
		void swap(Optional& other) {
		    char temp[sizeof(T)];
		    memcpy(temp, &_storage, sizeof(T));
		    memcpy(&_storage, &other._storage, sizeof(T));
		    memcpy(&other._storage, temp, sizeof(T));

		    bool tempHasValue = _hasValue;
		    _hasValue = other._hasValue;
		    other._hasValue = tempHasValue;
		}
    public:
        Optional() : _hasValue(false) {}

        Optional(const ft::nullopt_t&) : _hasValue(false) {}

        Optional(const T& data) : _hasValue(true) {
            new (_storage) T(data);
        }

        Optional(const Optional& other) : _hasValue(other._hasValue) {
             if (other._hasValue) {
                 new (_storage) T(*other.pointer());
                 _hasValue = true;
             } else {
                 _hasValue = false;
             }
        }
       Optional(T& data) : _hasValue(true) {
            new (_storage) T(data);
        }

        Optional(Optional& other) : _hasValue(other._hasValue) {
             if (other._hasValue) {
                 new (_storage) T(*other.pointer());
                 _hasValue = true;
             } else {
                 _hasValue = false;
             }
        }
        ~Optional() {
            reset();
        }

        Optional& operator=(const nullopt_t&) {
            reset();
            return *this;
        }
		Optional& operator=(const Optional&other) {
		    other.swap(*this);
		    return *this;
		}
        Optional& operator=(const T& value) {
            reset();
            _hasValue = true;
            new (_storage) T(value);
            return *this;
        }
		Optional& operator=(Optional&other) {
		    other.swap(*this);
		    return *this;
		}
        Optional& operator=(T& value) {
            reset();
            _hasValue = true;
            new (_storage) T(value);
            return *this;
        }
        void reset() {
            if (_hasValue) {
                pointer()->~T();
                _hasValue = false;
            }
        }
        bool has_value() const {
            return _hasValue;
        }

        T& value() {
            if (!_hasValue) throw std::runtime_error("No value");
            return *pointer();
        }

        const T& value() const {
            if (!_hasValue) throw std::runtime_error("No value");
            return *pointer();
        }

        T value_or(const T& defaultVal) const {
            return _hasValue ? value() : defaultVal;
        }

        T& operator*() {
            return value();
        }

        const T& operator*() const {
            return value();
        }

        T* operator->() {
            return pointer();
        }

        const T* operator->() const {
            return pointer();
        }

        operator bool() const {
            return _hasValue;
        }

       void emplace() {
           reset();
           _hasValue = true;
           new (_storage) T();
       }
       template<typename Arg1>
       void emplace(const Arg1& arg1) {
            reset();
            _hasValue = true;
            new (_storage) T(arg1);
       }
        template<typename Arg1, typename Arg2>
        void emplace(const Arg1& arg1, const Arg2& arg2) {
            reset();
            _hasValue = true;
            new (_storage) T(arg1, arg2);
        }
        template<typename Arg1, typename Arg2, typename Arg3>
        void emplace(const Arg1& arg1, const Arg2& arg2,const Arg3& arg3 ) {
            reset();
            _hasValue = true;
            new (_storage) T(arg1, arg2, arg3);
        }
    };
}

#endif
