#include<iostream>
#include<mutex>
#include<vector>


using namespace std;

mutex m;
void inkr(int &x) {
	unique_lock<mutex> unique_lock(m);
	this_thread::sleep_for(chrono::milliseconds(1));
	
	x++;
}
// problem trke
void inkrementiraj(int& x, int num_threads) {
	vector<thread> niti(1000);
	
	for (int i = 0; i < num_threads; i++) {
		niti[i] = thread(&inkr, ref(x));
	}
	for (int i = 0; i < num_threads; i++) {
		niti[i].join();
	}
}

void inkrementiraj_v1(int& x, int num_threads) {
	vector<thread> niti(1000);
	
	
	for (int i = 0; i < num_threads; i++) {
	
		
		niti[i] = thread(&inkr, ref(x));
	}
	for (int i = 0; i < num_threads; i++) {
		niti[i].join();
	}
}



int main() {
	int x = 0;
	int y = 0;
	inkrementiraj(y, 1000);
	inkrementiraj_v1(x, 1000);
	cout << x << endl;
	cout << y << endl;





	return 0;
}