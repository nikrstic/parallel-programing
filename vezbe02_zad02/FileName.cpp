#include<iostream>
#include<thread>

using namespace std;


template<typename T>
void kvadriraj(T& n) {
	n *= n;
	cout << "kvadrat: " << n;
}

template<typename T>
void saberi(T a, T b) {
	T n = a + b;
	cout << "zbir: " << n<< endl;
}

struct Osoba {
	string ime;

	string prezime;
	Osoba(string ime, string prezime) :ime(ime), prezime(prezime) {};
	friend ostream& operator << (ostream& output, const Osoba& a) {
		output << "osoba: " +a.ime + " " + a.prezime;
		return output;
	}
};

template<typename T>
T pomnozi(T a, T b) {
	return a * b;
	
}
void ispisi(Osoba& osoba) {
	cout << osoba << endl;
}


int main() {
	int n = 3;
	int m=5;
	int c;
	Osoba nik = Osoba("nikola", "krstic");
	thread t1(kvadriraj<int>, ref(n));
	thread t2(saberi<int>, n, m);
	thread t3(pomnozi<int>, n, m);
	cout <<"mainnn: " << nik << endl;
	thread t4(ispisi, ref(nik));
	thread t5([n, m](int& c) {
		c = n * n * m;
		cout << c << endl;
		}, ref(c));
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();




	return 0;
}