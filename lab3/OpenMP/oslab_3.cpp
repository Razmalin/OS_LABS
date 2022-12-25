#include <iostream>
#include <windows.h>
#include <omp.h.>

using namespace std;

const unsigned N = 100000000;
const unsigned blockSize = 60;			// 03050(6)
const unsigned threadsCount = 16;		// 1, 2, 4, 8, 12, 16

int main() {

	system("chcp 437");
	system("cls");
	cout << "This is a console application by Artem Ivanov,\n";
	cout << "a student of group 0305, for laboratory work\n";
	cout << "on the study of processor management using the OpenMP.\n\n";

	double pi = 0;
	int blocksNumber = N / blockSize;
	int start, finish;
	omp_lock_t locker;

	omp_set_num_threads(threadsCount);
	omp_init_lock(&locker);

	start = GetTickCount();

	#pragma omp parallel for schedule(dynamic, blockSize) reduction(+:pi)
	// calculate block using given formula
	for (int i = 0; i < N; ++i)
		pi += 4 / (1 + ((i + 0.5) / N) * ((i + 0.5) / N));

	finish = GetTickCount();

	pi /= N;

	cout.precision(N);
	int time = finish - start;
	cout << "Pi - " << pi << endl << "Time - " << time << " ms." << endl;

	return 0;
}
