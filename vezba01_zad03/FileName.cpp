#include<iostream>


using namespace std;
template<typename T>
void swap1(T& a, T& b) {
	T tmp = a;
	a = b;
	b = tmp;
}


template<typename T, typename F>
void sortiraj(T* niz, int n, F f) {
	for (int i = 0; i < n-1; i++) {
		for (int j = 0; j < n-i-1; j++) {
			if (f(niz[j], niz[j+1])) {
				swap1(niz[j], niz[j+1]);
			}
		}
	}
}
template<typename T>
struct Uporedi{
	bool operator()(T& a, T& b) {
		if (a > b) {
			return true;
		}
		return false;
	}
};


int main() {
	int niz[] = {4,2,1,2,1,9,5,3,2};
	Uporedi<int> f;
	sortiraj(niz , 9, f);
	for (int i = 0; i < 9; i++) {
		cout << niz[i]<<" ";
	}
	int a = 5;
	int b = 19;
	swap1(a, b);
	cout << a << " -  " << b << endl;





	return 0;
}