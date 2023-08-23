#include "../HashTable.hpp"

class a{
	public:
		size_t hash(void) const { 
			return 0;
		};
};
class b{

};

void foo(){
	ft::HashTable<std::string, int> a;
	a.insert("1", 3);
	a.insert("13", 7);
	std::cout << a.get("1").value_or(-1) << '\n';
	std::cout << a.get("13").value_or(-1) << '\n';
	std::cout << a.get("14").value_or(-1) <<'\n';
	for (ft::HashTable<std::string,int>::iterator it = a.begin(); it != a.end(); ++it){
		std::cout << (*it)->second << '\n';
	}
	std::cout << "Int \n";
	ft::HashTable<int, int > b;
	b.insert(1, 3);
	b.insert(4, 7);
	std::cout << b.get(1).value_or(-1) << '\n';
	std::cout << b.get(4).value_or(-1) << '\n';
	std::cout << b.get(7).value_or(-1) <<'\n';
	for (ft::HashTable<int,int>::iterator it = b.begin(); it != b.end(); ++it){
		std::cout << (*it)->second << '\n';
	}	// std::cout << ft::type::is_std_string<std::string>::value << '\n';
}
int main(void) {
	// ft::HashTable<a, int> x;
	// ft::HashTable<std::string,int> x;
	ft::HashTable<b, int> x;
	// foo();
	system("leaks a.out");
}