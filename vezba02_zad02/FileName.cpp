#include<iostream>

using namespace std;


class Tacka2d {
public:
	int x;
	int y;
public:
	Tacka2d() = default;
	Tacka2d(int x = 0, int y = 0) :x(x), y(y) {};
	bool operator < (const Tacka2d a) { 
		if (this->x < a.x)return 0;
	
	}
	bool operator == (const Tacka2d a) {
		if (a.x == this->x && a.y == this->y) return true;
		return false;
	}
	bool operator <= (const Tacka2d a) {
		return operator<(a) || operator==(a);
	}
	bool operator >(const Tacka2d a) {
		return !( operator <=(a));
	}
	bool operator >=(const Tacka2d a) {
		return !operator<(a);
	}
	bool operator !=(const Tacka2d a) {
		return !operator==(a);
	}
	friend ostream &operator<<(ostream &output, Tacka2d a) {
		output << "koordinate tacke: " << a.x << "i" << a.y;
		return output;
	}
	Tacka2d operator+(const Tacka2d a) {
		return Tacka2d(a.x + this->x, a.y + this->y);
	}
	Tacka2d operator- (const Tacka2d a) {
		return Tacka2d(this->x - a.x, this->y - a.y);
	}
};



int main() {
	Tacka2d a = Tacka2d(3, 7);
	Tacka2d b = Tacka2d(7, 3);
	
	cout << a << endl;
	cout << b << endl;
	cout << "a+b" << a + b << endl;
	cout << "a-b" << a - b << endl;
	cout << (a > b)<< endl;
	cout << (a >= b)<< endl;
	cout << (a < b)<< endl;
	cout << (a <= b)<< endl;
	cout << (a == b)<< endl;
	cout << (a != b)<< endl;










	return 0;
}