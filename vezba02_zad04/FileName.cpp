#include<iostream>
#include<thread>
#include<functional>
#include<iterator>
#include<numeric>
#include<vector>
using namespace std;

template<typename Iterator, typename T>
void racunanje(int* niz, Iterator start, Iterator end, int& rez) {
	rez = accumulate(start, end, 0);
}


template<typename Iterator, typename T>
int main() {
	vector<int> niz = { 100,10,10,5,5,6,1,1,1,2, 3, 4, 1, 0, 0, 2, 1, 0, 0, 4, 1, 1, 1, 1,1 };
	int n = sizeof(niz) / sizeof(niz[0]);
	cout << "n:" << n << endl;
	int num_threads = 8;
	int donji_ceo_deo = n / num_threads;
	vector<thread> niti(num_threads);
	vector<int> medjurezultati(num_threads - 1, 0);
	Iterator start=0;
	int sum = 0;
	int cela_suma = 0;
	for (int i = 0; i < num_threads - 1; i++) {
		Iterator end = start;
		advance(end, donji_ceo_deo);

		niti[i] = thread(&racunanje<niz.begin(),int>, ref(niz), start, end, ref(medjurezultati[i]));
		start = end;
	}
	for (thread& a : niti) {
		a.join();
	}
	for (int a : medjurezultati) {
		//cout << "a:" << a << endl;
		cela_suma += a;
	}
	racunanje<Iterator, int>(niz, start, n - 1, sum);
	//cout << suma(niz, 0, n);
	cela_suma += sum;
	
	//for_each(niti.begin(), niti.end(), niti::join());


	cout << "cela suma: " << cela_suma << endl;












	return 0;
}