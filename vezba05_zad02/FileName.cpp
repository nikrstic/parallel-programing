#include<iostream>
#include<thread>
#include<omp.h>
#include<chrono>


using namespace std;
using namespace std::chrono;

void v1() {
	long long num_steps = 1 << 24;
	double step = 1.0 / num_steps;
	double pi = 0;
	int num_threads = 8;
	#pragma omp parallel num_threads(num_threads) shared(pi, num_threads,step, num_steps)
	{
		double partial_sum = 0;
		int thread_id = omp_get_thread_num();
		for (long long i = thread_id; i < num_steps; i += num_threads) {
			double x = (i + 0.5) * step;
			partial_sum += 4.0 / (1.0 + x * x);
		}
		partial_sum *= step;

	#pragma omp atomic
		{
			pi += partial_sum;
		}
	}
	printf("pi = %lf\n", pi);
	cout << pi;
	




}

void v2() {
	// Prethodna implementacija bila je relativno komplikovana. 
	// Lepota korišćenja omp-a je mogućnost paralelizacije bez
	// značajnog (skoro pa ikakvog) usložnjavanja koda.
	long long num_steps = 1 << 24;
	double step = 1.0 / num_steps;
	double pi = 0;
	int num_threads = 8;
	auto start_t = high_resolution_clock::now();
	#pragma omp parallel num_threads(num_threads) \
shared(num_steps, step, pi, num_threads)
	{
		int tid = omp_get_thread_num();
		double partial_sum = 0;
		// Ako svaka nit može da se identifikuje, onda možemo
		// svakoj niti da dodelimo odgovarajuće "parče" posla - 
		// to smo uradili u prethodnoj implementaciji. 
		// Međutim, bitna stvar je da indeksi nad kojima
		// određena nit treba da obavi neku računicu
		// ne moraju da budu uzastopni. Veoma laka implementacija,
		// a u slučaju korišćenja nizova može poboljšati i korišćenje
		// keš memorije, je da nit sa indeksom
		// tid krene sa "poslom" upravo od indeksa tid i preskače po tnum
		// indeksa (tnum = broj niti) u petlji koja ide do n, gde je n broj
		// indeksa (veličina niza recimo i sl.)
		for (long long i = tid; i < num_steps; i += num_threads) {
			double x = (i + 0.5) * step;
			partial_sum += 4.0 / (1.0 + x * x);
		}
		partial_sum *= step;
		#pragma omp atomic
		pi += partial_sum;
	}
	auto end_t = high_resolution_clock::now();
	auto micro_secs = duration_cast<microseconds>
		(end_t - start_t).count();
	printf("\nmicro seconds = %ld, pi = %lf\n", micro_secs, pi);
}




int main() {
	v1();
	v2();







	return 0;
}