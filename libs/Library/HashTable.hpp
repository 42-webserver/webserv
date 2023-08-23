#ifndef HASHTABLE_HPP
# define HASHTABLE_HPP

# include <iostream>
# include <vector>
# include "Optional.hpp"
# include "Type.hpp"
# include "Assert.hpp"
# include <map>
# include <ctime>
# include <cstdlib>
# include <unordered_set>

namespace ft{


class cachePrime {
private:
    static const size_t _size;
    static std::map<size_t, bool> _primes;

    cachePrime();

public:
    static bool isPrime(size_t n) {
		if (n >= _size) throw std::runtime_error("Count Prime size is 10000000");
        return _primes.find(n) != _primes.end();
    }

    static size_t nextPrime(size_t n) {
		if (n >= _size) throw std::runtime_error("Count Prime size is 10000000");
        std::map<size_t, bool>::iterator it = _primes.lower_bound(n);
        if (it != _primes.end()) {
            return it->first;
        }
        return 0;
    }

    static void initialize() {
        bool *sieve = new bool[_size];
        for (size_t i = 0; i < _size; ++i) sieve[i] = true;
        sieve[0] = sieve[1] = false;

        for (size_t i = 2; i < _size; ++i) {
            if (sieve[i]) {
                _primes[i] = true;
                for (size_t j = i * i; j < _size; j += i) {
                    sieve[j] = false;
                }
            }
        }
        delete[] sieve;
    }
};

const size_t cachePrime::_size = 1000000;
std::map<size_t, bool> cachePrime::_primes;

struct cachePrimeInitializer {
    cachePrimeInitializer() {
        cachePrime::initialize();
    }
} _init;

const double maxLoadFactor = 0.75;

//Basic HashTable
template<typename KeyType, typename ValueType>
class HashTable {
public:
	typedef ValueType                                                 value_type;
    typedef KeyType                                                   key_type;
private:
	typedef typename std::vector<ft::Optional<std::pair<KeyType, ValueType> > > Table;
	Table _table;
	size_t _capacity;
	size_t _itemCount;	
	size_t _hash1(const KeyType& key, ft::type::false_type) const {
        return key.hash(key) % _capacity;
    }

    size_t _hash1(const KeyType& key, ft::type::true_type)  const {
        return key % 31 % _capacity;
    }

    size_t _hash1(const KeyType& key) const {
        return _hash1(key, ft::type::integral_constant<bool, 
		ft::type::is_integral<KeyType>::value>());
    }
	size_t _hash2(size_t hash1) const {
		size_t hash = hash1 * 0x1f3d5b79 + 0x8d2939;
		hash = (hash == 0 ? 1 : hash);
        return hash % _capacity;
	}	
private:
	void	_checkTrait() {
		ft::Assert::current_level = ft::Assert::COMPILE;
		ASSERT(ft::type::is_integral<KeyType>::value 
		||  ft::type::is_std_string<KeyType>::value
		|| ft::type::hasHashFunction<KeyType>::value,
		"No Hash Function or No integral");
	}
	size_t	_nextPrime(size_t n) {
		return cachePrime::nextPrime(n);
	}
	void _resize() {
		size_t oldCapacity = _capacity;
		_capacity = ft::cachePrime::nextPrime(2 * oldCapacity);

  		Table newTable(_capacity);
        _table.swap(newTable);
		
		_itemCount = 0;
		typedef typename Table::iterator Iter;
		for (Iter it = newTable.begin(); it != newTable.end(); it++) {
			if ((*it).has_value()) {
				this->insert((*it)->first, (*it)->second);
			}
		}
		}
public:
	HashTable() : _capacity(ft::cachePrime::nextPrime(1000)), _table(ft::cachePrime::nextPrime(1000)) {
		_checkTrait();
	}
public:
	class iterator : public std::iterator<std::forward_iterator_tag, std::pair<KeyType, ValueType> >{
		private:
			typedef std::pair<KeyType, ValueType> value_type;
			typedef typename std::vector<ft::Optional<value_type> >::iterator Iter;
			Iter _it;
			Iter _endIt;
		public:
			iterator(Iter start, Iter end) : _it(start), _endIt(end) {
				while (_it != _endIt && !(*_it).has_value()){
					++_it;
				}
			}
			value_type& operator*() { return *(_it->operator->());}
			value_type* operator->() { return _it->operator();}

			iterator& operator++() {
				do {
					++_it;
				} while (_it != _endIt && !(*_it).has_value());
				return *this;
			}

			bool operator!=(const iterator& other) const {
				return _it != other._it;
			}
	};

public:
	HashTable(size_t cap) : _capacity(ft::cachePrime::nextPrime(cap)), _table(_capacity) {}
	
	iterator begin() {
		return iterator(_table.begin(), _table.end());
	}
	iterator end() {
		return iterator(_table.end(), _table.end());
	}
    void insert(const KeyType& key, const ValueType& value) {
        if ((1.0 * _itemCount) / _capacity >= maxLoadFactor) {
            _resize();
        }
        size_t hash1 = _hash1(key);
        size_t hash2 = _hash2(hash1);

        for (size_t i = 0; i < _capacity; ++i) {
            size_t pos = (hash1 + i * hash2) % _capacity;
            if (!_table[pos].has_value() || _table[pos]->first == key) {
                if (!_table[pos].has_value()) {
                    _itemCount++;
                }
                _table[pos] = std::make_pair(key, value);
                return;
            }
        }
    }

    void remove(const KeyType& key) {
        size_t hash1 = _hash1(key);
        size_t hash2 = _hash2(hash1);

        for (size_t i = 0; i < _capacity; ++i) {
            size_t pos = (hash1 + i * hash2) % _capacity;
            if (_table[pos].has_value() && _table[pos]->first == key) {
                _table[pos] = ft::nullopt;
                _itemCount--;
                return;
            }
        }
    }

    ft::Optional<ValueType> get(const KeyType& key) const {
        size_t hash1 = _hash1(key);
        size_t hash2 = _hash2(hash1);

        for (size_t i = 0; i < _capacity; ++i) {
            size_t pos = (hash1 + i * hash2) % _capacity;
            if (!_table[pos].has_value()) {
                return ft::nullopt; 
            }
            if (_table[pos]->first == key) {
                return _table[pos]->second;
            }
        }

        return ft::nullopt;
    }
};


//std::string Specialization
template<typename ValueType>
class HashTable<std::string, ValueType> {
public:
	typedef ValueType                                                 val_type;
    typedef std::string                                               key_type;
private:
	typedef typename std::vector<ft::Optional<std::pair<key_type, ValueType> > > Table;
	Table _table;
	size_t _capacity;
	size_t _itemCount;
	unsigned int _seed;
	size_t _hash1(const key_type& key ) const {
		return MurmurHash3(key, _seed) % _capacity;
	}
	size_t _hash2(size_t hash1) const {
		size_t hash = hash1 * 0x1f3d5b79 + 0x8d2939;
		hash = (hash == 0 ? 1 : hash);
        return hash % _capacity;
	}
private:
	//murmur hash
		inline unsigned int _rotl(unsigned int value, int shift) const
		{
		    return (value << shift) | (value >> (32 - shift));
		}

		unsigned int get_32bit_Block(std::string stream, int block_no) const
		{
			return	 stream[(block_no * 4) + 0] +
					(stream[(block_no * 4) + 1] << 8) +
					(stream[(block_no * 4) + 2] << 16) +
					(stream[(block_no * 4) + 3] << 24);
		}

		// Finalization mix - force all bits of a hash block to avalanche
		unsigned int finalize_hash_value(unsigned int h) const
		{
			h ^= h >> 16;
			h *= 0x85ebca6b;	// 2,246,822,507
			h ^= h >> 13;
			h *= 0xc2b2ae35;	// 3,266,489,909
			h ^= h >> 16;

			return h;
		}


		//MurmurHash3_x86_32bit
		unsigned int MurmurHash3(std::string data, unsigned int seed) const
		{
			unsigned int h1 = seed;

			//for x86 32bit variant of Murmur3 hash function, 2 constants are required.
			const unsigned int c1 = 0xcc9e2d51;		// 3,432,918,353
			const unsigned int c2 = 0x1b873593;		// 461,845,907

			const int r1 = 15;
			const int r2 = 13;

			const int m = 5;
			const int n = 0xe6546b64;	//3,864,292,196

			const int remaining_Bytes = data.length() & 3; // data.length() % 4
			const int total_32bit_blocks = (data.length() - remaining_Bytes) / 4;	//total 32bit blocks


			for (int block_no = 0; block_no < total_32bit_blocks; block_no++)
			{
				unsigned int k1 = get_32bit_Block(data, block_no);
				k1 *= c1;
				k1 = _rotl(k1, r1);	//ROTL32
				k1 *= c2;

				h1 ^= k1;
				h1 = _rotl(h1, r2);
				h1 = h1 * m + n;
			}

			unsigned int k1 = 0;

			switch (remaining_Bytes)
			{
			case 3: k1 ^= data[(total_32bit_blocks * 4) + 2] << 16;
			case 2: k1 ^= data[(total_32bit_blocks * 4) + 1] << 8;
			case 1: k1 ^= data[(total_32bit_blocks * 4) + 0];

				k1 *= c1; 
				k1 = _rotl(k1, r1); 
				k1 *= c2;
				h1 ^= k1;
			};

			h1 ^= data.length();
			h1 = finalize_hash_value(h1);

			return h1;
		}
	//

	void	_checkTrait() {
		ft::Assert::current_level = ft::Assert::COMPILE;
		ASSERT(
		ft::type::is_std_string<key_type>::value
		,"No Hash Function or No integral");
	}
	size_t	_nextPrime(size_t n) {
		return cachePrime::nextPrime(n);
	}
	void _resize() {
		size_t oldCapacity = _capacity;
		_capacity = ft::cachePrime::nextPrime(2 * oldCapacity);

  		std::vector<ft::Optional<std::pair<key_type, ValueType> > > newTable(_capacity);
        _table.swap(newTable);
		
		_itemCount = 0;
		typedef typename Table::iterator Iter;
		for (Iter it = newTable.begin(); it != newTable.end(); it++) {
			if ((*it).has_value()) {
				insert((*it)->first, (*it)->second);
			}
		}
		}
public:
	HashTable() : _capacity(ft::cachePrime::nextPrime(1000)), _table(ft::cachePrime::nextPrime(1000)) {
		_checkTrait();
		std::srand(std::time(0));
   		_seed = std::rand();

	}
public:
	class iterator : public std::iterator<std::forward_iterator_tag, std::pair<key_type, ValueType> >{
		private:
			typedef std::pair<key_type, ValueType> value_type;
			typedef typename std::vector<ft::Optional<value_type> >::iterator Iter;
			Iter _it;
			Iter _endIt;
		public:
			iterator(Iter start, Iter end) : _it(start), _endIt(end) {
				while (_it != _endIt && !(*_it).has_value()){
					++_it;
				}
			}
			value_type& operator*() { return *(_it->operator->());}
			value_type* operator->() { return _it->operator();}

			iterator& operator++() {
				do {
					++_it;
				} while (_it != _endIt && !(*_it).has_value());
				return *this;
			}

			bool operator!=(const iterator& other) const {
				return _it != other._it;
			}
	};

public:
	HashTable(size_t cap) : _capacity(ft::cachePrime::nextPrime(cap)), _table(_capacity) {}
	
	iterator begin() {
		return iterator(_table.begin(), _table.end());
	}
	iterator end() {
		return iterator(_table.end(), _table.end());
	}
    void insert(const key_type& key, const ValueType& value) {
        if ((1.0 * _itemCount) / _capacity >= maxLoadFactor) {
            _resize();
        }

        size_t hash1 = _hash1(key);
        size_t hash2 = _hash2(hash1);

        for (size_t i = 0; i < _capacity; ++i) {
            size_t pos = (hash1 + i * hash2) % _capacity;
            if (!_table[pos].has_value() || _table[pos]->first == key) {
                if (!_table[pos].has_value()) {
                    _itemCount++;
                }
                _table[pos] = std::make_pair(key, value);
                return;
            }
        }
    }

    void remove(const key_type& key) {
        size_t hash1 = _hash1(key);
        size_t hash2 = _hash2(hash1);

        for (size_t i = 0; i < _capacity; ++i) {
            size_t pos = (hash1 + i * hash2) % _capacity;
            if (_table[pos].has_value() && _table[pos]->first == key) {
                _table[pos] = ft::nullopt;
                _itemCount--;
                return;
            }
        }
    }

    ft::Optional<ValueType> get(const key_type& key) const {
        size_t hash1 = _hash1(key);
        size_t hash2 = _hash2(hash1);

        for (size_t i = 0; i < _capacity; ++i) {
            size_t pos = (hash1 + i * hash2) % _capacity;
            if (!_table[pos].has_value()) {
                return ft::nullopt; 
            }
            if (_table[pos]->first == key) {
                return _table[pos]->second;
            }
        }

        return ft::nullopt;
    }
};


}

#endif