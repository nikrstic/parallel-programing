#include<iostream>
#include<vector>
#include<random>
#include<omp.h>

using namespace std;



int main() {
	vector<int> a(1<<24, rand());
	vector<int> b(1<<24, rand()%100);
	vector<int> c(1<<24, 0);

	int num_threads = 8;
	int n = distance(a.begin(), a.end());
	#pragma omp parallel num_threads(num_threads) shared(a,b,c,num_threads,n)
	{
		#pragma omp for nowait
		{
			for (int i = 0; i < n; i++) {
				c[i] = a[i] + b[i];
			}
		}
	
	
	}
	cout << a[0] << " " << b[0]<<endl;
	/*
	for (int i = 0; i < n; i++) {
		cout<<c[i]<<endl;
	}*/




	return 0;
}