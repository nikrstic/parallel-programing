#include<iostream>
#include<mutex>
#include<condition_variable>
#include<vector>
#include<random>
#include<time.h>

using namespace std;
mutex m;
condition_variable cv;
vector<int> niz;
template<typename T>
void producer() {
	while (true) {
		this_thread::sleep_for(chrono::milliseconds(1500));
		unique_lock<mutex> ul1(m);

		cv.notify_one();
		niz.push_back(rand());
	}
}

template<typename T>
void consumer() {
	while (true) {
		unique_lock<mutex> ul2(m);
		cv.wait(ul2, []() {return !niz.empty(); });
		cout << niz.back() << endl;
		niz.pop_back();
		
	}
}
void probaj() {
	thread t1 (producer<int>);
	thread t2(consumer<int>);
	thread t3 (consumer<int>);
	thread t4 (consumer<int>);
	thread t5 (consumer<int>);
	thread t6 (consumer<int>);
	thread t7 (consumer<int>);
	
	t1.join();
	t2.join();
	t3.join();

}

int main() {
	
	vector<int> niz;
	probaj();
	




	return 0;
}