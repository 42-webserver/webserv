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
            if (_hasValue) {
                new (_storage) T(*other.pointer());
            }
        }

        ~Optional() {
            reset();
        }

        Optional& operator=(const nullopt_t&) {
            reset();
            return *this;
        }
		Optional& operator=(Optional other) {
		    other.swap(*this);
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
    };
}

#endif
