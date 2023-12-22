#include<iostream>
#include<stack>
#include<mutex>
#include<random>
using namespace std;

struct stack_is_empty :exception {
	const char* what() const { return "stack is empty!"; }
};

template<typename T>
struct threadSafeStack {
	stack<T> s;
	mutex m;
	threadSafeStack() = default;
	threadSafeStack operator= (threadSafeStack&) = delete;
	threadSafeStack(const threadSafeStack& other) {
		unique_lock<mutex> ul(other.m);
		this->s = other.s;
	};
	bool empty() {
		unique_lock<mutex> ul(this->m);
		return s.empty();
	}
	void pop(T& value) {
		unique_lock<mutex> ul(this->m);
		if (s.empty) throw stack_is_empty;
		value = s.top();
		s.pop();
	}
	shared_ptr<T> pop() {
		unique_lock<mutex> ul(this->m);
		if (s.empty()) throw stack_is_empty();
		shared_ptr<T> tmp = make_shared<T>(s.top());
		s.pop();
		return tmp;
	}
	void push(T new_value) {
		unique_lock<mutex> ul(this->m);
		s.push(new_value);
	}
};
threadSafeStack<int> tss;

void producer() {
	while (true) {
		this_thread::sleep_for(chrono::milliseconds(500));
		tss.push(rand());
	}
}
void consumer() {
	while (true) {
		this_thread::sleep_for(chrono::milliseconds(100));
		if (!tss.empty()) {
			try {
				cout << *(tss.pop()) << endl;
			}
			catch (stack_is_empty e) {
				cout << e.what() << endl;
			}
		}
	}
}

int main() {
	thread t1(producer);
	thread t2(consumer);
	thread t3(consumer);

	t1.join();
	t2.join();
	t3.join();

	return 0;
}