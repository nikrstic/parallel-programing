#include<iostream>
#include<mutex>
#include<queue>
#include<condition_variable>


using namespace std;

template<typename T>
class threadsafe_queue {
	queue<T> q;
	mutex m;
	condition_variable cv;
public:
	threadsafe_queue() = default;
	threadsafe_queue operator=(const threadsafe_queue&) = delete;
	threadsafe_queue(const threadsafe_queue& drugi) {
		unique_lock<mutex> ul(drugi.m);
		this->q = drugi.q;

	}
	bool empty() {
		unique_lock<mutex> ul(m);
		return q.empty();
	}
	void push(T new_value) {
		unique_lock<mutex> ul(m);
		q.push(new_value);
		cv.notify_one();
	}
	bool try_pop(T& value) {
		unique_lock<mutex> ul(m);
		if (!q.empty()) {
			value = q.front();
			q.pop();
			return true;
		}
		return false;

	}
	shared_ptr<T> try_pop() {
		unique_lock<mutex> ul(m);
		if (!q.empty()) {
			shared_ptr<T> value = make_shared(q.front());
			q.pop();
			return value;
		}
		return false;
	}
	void wait_and_pop(T& value) {
		unique_lock<mutex> ul(m);
		cv.wait(ul, [q]() {return !q.empty(); });
		value = q.front();
		q.pop();

	}
	shared_ptr<T> wait_and_pop() {
		unique_lock<mutex> ul(m);
		cv.wait(ul, [this]() {return !(q.empty()); });
		shared_ptr<T> value = make_shared<T>(move(q.front()));
		q.pop();
		
		return value;

	}


};

threadsafe_queue<int> tsq;

void producer() {
	while (true) {
		this_thread::sleep_for(chrono::milliseconds(500));
		tsq.push(rand() % 1000);
	}
}

void consumer() {
	while (true) {
		// this_thread::sleep_for(chrono::microseconds(100));
		cout << *(tsq.wait_and_pop()) << endl;
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