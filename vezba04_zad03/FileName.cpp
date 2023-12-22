#include<iostream>
#include<thread>
#include<mutex>
#include<vector>
#include<atomic>
#include<numeric>
#include<math.h>
using namespace std;

template<typename T, typename Iterator>
void accumulate_parallel(Iterator start,Iterator end,atomic<T>& sum) 
{
	T result = accumulate(start, end, 0, [](int a, int b) {return a + b * b; });
	sum += result;
	
}

int main() {
	int thread_num = 8;
	//vector<int> niz = { 1,2,3,4,5,6,1,2,3 };
	int n = 1000;
	vector<int> niz(n);
	for (int i = 0; i < n; i++)
		niz[i] = rand() % 20;


	//int n = distance(niz.begin(), niz.end());
	int donji_ceo_deo = (n +thread_num -1) / thread_num;
	vector<thread> niti(thread_num-1);
	atomic<int> sum = 0;
	auto start = niz.begin();
	auto end = start;
	
	for (int i = 0; i < thread_num - 1; i++) {
		advance(end, donji_ceo_deo);
		auto instanca = accumulate_parallel<int, vector<int>::iterator>;
		niti[i] = thread(instanca, start, end, ref(sum));
		start = end;
	}
	
	
	accumulate_parallel(start, end, sum);
	
	for (auto & nit : niti) {
		nit.join();
	}
	cout<<sqrt((double)sum);





	return 0;
}