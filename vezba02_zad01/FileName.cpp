#include<iostream>
#include<thread>

using namespace std;

void ispisi(string tekst) {

	cout << tekst << endl;
	cout << this_thread::get_id();
}

int main() {

	string tekst="neki tekst koji ispisuju niti";

	thread t1(ispisi, tekst);
	thread t2(ispisi, tekst);

	t1.join();
	t2.join();

	return 0;
}