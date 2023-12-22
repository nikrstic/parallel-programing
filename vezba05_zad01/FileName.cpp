#include<iostream>
#include<thread>


using namespace std;
template<typename T>
void swap1(T& a, T& b) {
	T tmp = a;
	a = b;
	b = tmp;
}
template<typename T>
int partition(T* niz, int l, int r) {
	T pivot = niz[r];
	int i = l;
	for (int j = l; j <= r; j++) {
		if (niz[j] < pivot)
		{
			i++;
			swap1(niz[i], niz[j]);
		}
	}
	swap1(niz[r], niz[i + 1]);
	return i + 1;
}
template<typename T>
void quicksort(T* array, int l, int r)
{
	T pivot = partition(array, l, r);
	if (r - l > 1024 * 8) {
		thread t1(&quicksort<T>, array, l, pivot - 1);
		quicksort(array, pivot + 1, r);
		t1.join();
	}
	else {
		quicksort(array, l, pivot - 1);
		quicksort(array, pivot + 1, r);
	}
}




int main() {
	long long n = 1 << 20;
	int* array = (int*)malloc(n * sizeof(int));
	for (long long i = 0; i < n; i++)
		array[i] = rand();
	auto start_t = chrono::high_resolution_clock::now();
	quicksort(array, 0, n - 1);
	auto end_t = chrono::high_resolution_clock::now();
	cout << chrono::duration_cast<chrono::microseconds>
		(end_t - start_t).count() << endl;

	free(array);
	return 0;







	return 0;
}