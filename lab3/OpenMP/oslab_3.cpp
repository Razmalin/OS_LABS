#include <iostream>
#include <omp.h.>
#include <windows.h>
#include <iomanip>

using namespace std;

const int threadsCount = 8;		// 1, 2, 4, 8, 12, 16
const int blockSize = 60; 		// 03050(6)
const int N = 100000000;

double pi = 0;
int main()
{
	system("chcp 437");
	system("cls");
	cout << "This is a console application by Artem Ivanov,\n";
	cout << "a student of group 0305, for laboratory work\n";
	cout << "on the study of processor management using the OpenMP.\n\n";

	int CountBlock = N / blockSize;
	int startTime, endTime;
	omp_lock_t lock;
	omp_set_num_threads(threadsCount);
	omp_init_lock(&lock);
	startTime = GetTickCount();
#pragma omp parallel  
	{
		double keepingPi = 0;
#pragma omp for
		for (int block = 0; block < CountBlock; ++block)
			for (int i = block * blockSize; i < block * blockSize + blockSize; ++i)
				keepingPi += 4 / (1 + ((i + 0.5) / N) * ((i + 0.5) / N));
		omp_set_lock(&lock);
		pi += keepingPi;
		omp_unset_lock(&lock);
	}
	endTime = GetTickCount();
	pi /= N;
	cout << "Pi - " << fixed << setprecision(20) << pi;
	cout <<	endl << "Time -  " << endTime - startTime << " ms";
}
