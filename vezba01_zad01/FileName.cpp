#include<iostream>

using namespace std;

template<typename T>
struct Kvadriraj {
	T operator()(T a){
		return a * a;
	}
};

template<typename T>
struct Saberi {
	T operator()(T a, T b) {
		return a + b;
	}
};
template<typename T>
struct manjeIliJendako {
	T operator()(T a, T b) {
		return a <= b;
	}
};


int main() {
	Kvadriraj<int> kvadrat;
	int a = kvadrat(3);
	cout<< a <<endl;
	Saberi<int> sabiraj;
	cout << sabiraj(5, 6) << endl;
	manjeIliJendako<int> poredi;
	cout << poredi(5, 4);
	auto b = [](int a)  {return a * a; };
	auto c = [](int a, int b) {return a + b;  };
	cout<<endl<<b(4)<<endl;
	cout<<c(2,3)<<endl;
	auto d = [](int a, int b) {return a <= b; };
	cout << d(5, 7) << endl;
	return 0;
}