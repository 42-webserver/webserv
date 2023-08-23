#include "../HashTable.hpp"

class a{
	public:
		size_t hash(void) const { 
			return 0;
		};
};
class b{

};
int main(void) {
	// ft::HashTable<a, int> x;
	// ft::HashTable<std::string,int> x;
	// ft::HashTable<b, int> x;
	ft::HashTable<std::string, int> a;
	a.insert("1", 3);
	a.insert("13", 7);
	std::cout << a.get("1").value_or(-1) << '\n';
	std::cout << a.get("13").value_or(-1) << '\n';
	std::cout << a.get("14").value_or(-1) <<'\n';
	std::cout << ft::type::is_std_string<std::string>::value << '\n';
}