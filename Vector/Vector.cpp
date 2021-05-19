#include"Vector.h"
#include<iostream>

int main() {

	Vector<int> v{ 1,2,3,4,5 };
	Vector<int> v1{};
	v1 = v;
	std::cout << v1.size() << " " << v1.capacity() << '\n';
	v.print(std::cout);

	//test more here :)

	return 0;
}