#include<iostream>

using namespace std;

template<typename T>
class Niz {
public:
	T prvi[100];
public:
	T operator[] (int a) const {
		return prvi[a];
	}
	T& operator[] (int a) { 
		return prvi[a];
	}
	struct Iterator {
		using vrsta = output_iterator_tag;
		using value_type = int;
		using pointer = prvi*;
		using reference = int&;

		//T operator[](int a) { return prvi[a]; };
		pointer operator ->() { return prvi; }
		reference operator*() { return *prvi; }
		Iterator& operator++() { prvi++; return *this; }
		Iterator& operator++(int) { T tmp = *prvi; prvi++; return tmp; }
		bool operator == (const Iterator& a) { return prvi* == a; }
		bool operator != (const Iterator& a) { return prvi* != a; }
		

	};
	Iterator begin() { return Iterator(&prvi); };
	Iterator end() { return Iterator(prvi + 3); };





};


int main() {
	
	Niz<int> niz;
	niz[0] = 1;
	niz[1] = 4;
	niz[2] = 5;
	for (int el : niz) {
		cout << el << " ";
	}








	return 0;
}