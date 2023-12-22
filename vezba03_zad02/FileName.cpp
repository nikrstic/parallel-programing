#include<iostream>
#include<mutex>
#include<thread>
#include<cstring>
using namespace std;

mutex m;
struct Racun {
	int iznos;
	string id;
	mutex m;

	Racun(int iznos, string id) : iznos(iznos), id(id) {};

};

void transakcija(Racun& sa, Racun& na, int iznos) {
	unique_lock<mutex> ul(sa.m,defer_lock);
	unique_lock<mutex> ul1(na.m, defer_lock);
	lock(ul1, ul);
	sa.iznos -= iznos;
	na.iznos += iznos;
	
	cout << "prebaceno sa:" << sa.id << " na " << na.id;
}


int main() {
	Racun prvi(1000, "jds");
	Racun drugi (2000, "assd");
	transakcija(prvi, drugi, 500);
	cout<<endl << "prvi: " << prvi.iznos << "drugi: " << drugi.iznos << endl;




	return 0;
}