#include<iostream>
#include<thread>
#include<vector>
#include<algorithm>
using namespace std;


void suma(int* niz, int a, int b, int& sum)
{
	for (int i = a; i < b; i++) {
		sum += niz[i];
	}
	
}


int main() {
	int niz[] = { 100,10,10,5,5,6,1,1,1,2, 3, 4, 1, 0, 0, 2, 1, 0, 0, 4, 1, 1, 1, 1,1 };
	int n = sizeof(niz) / sizeof(niz[0]);
	cout << "n:" << n << endl;
	int num_threads = 8;
	int donji_ceo_deo = n  / num_threads;
	vector<thread> niti(num_threads);
	vector<int> medjurezultati(num_threads-1,0);
	int start=0, end=0, sum=0;
	int cela_suma = 0;
	for (int i = 0; i < num_threads - 1; i++) {
		end += donji_ceo_deo;
		niti[i] = thread(&suma, ref(niz), start, end,ref(medjurezultati[i]));

		start=end;
	}
	for (thread& a : niti) {
		a.join();
	}
	
	suma(niz, start, n - 1, sum);
	//cout << suma(niz, 0, n);
	cela_suma += sum;
	for (int a : medjurezultati) {
		//cout << "a:" << a << endl;
		cela_suma += a;
	}
	//for_each(niti.begin(), niti.end(), niti::join());


	cout <<"cela suma: " << cela_suma << endl;












	return 0;
}