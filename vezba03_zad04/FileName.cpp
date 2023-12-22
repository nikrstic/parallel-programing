#include<iostream>
#include<vector>
#include<future>


using namespace std;

template<typename Iterator>
void nadji_max(Iterator start, Iterator end, int &max ) {
	max = *start;
	int n = distance(start, end);
	for (auto it = start; it != end; it++) {
		if (*it > max) {
			max = *it;
		}
	}
	
}

template<typename IteratorT>
typename IteratorT::value_type main1() {
	vector<int> niz = { 1,2,32,4,5 };
	int n = niz.size();
	auto start= niz.begin();
	auto end = start;
	int max;
	nadji_max(start, niz.end(), max);
	cout << max;
	int num_threads = 8;
	int donji_ceo_deo = (n- 1) / num_threads;
	vector<future<void>> niti(num_threads);
	vector<int> max_dela(num_threads, 0);
	int i;
	for (i = 0; i < num_threads-1; i++) {

		advance(end,num_threads);
		niti[i] = async(&nadji_max<IteratorT>,start,end,ref(max_dela[i]));
		start = end;
	
	}
	int a;
	auto real_end = niz.end();
	nadji_max(start, real_end, a);
	int maxx;
	nadji_max( max_dela.begin(), max_dela.end(),maxx);
	maxx = maxx > a ? maxx : a;
	cout << maxx;
}

int main() {
	main1<std::vector<int>::iterator>();

	return 0;
}