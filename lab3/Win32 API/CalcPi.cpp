
#include <iostream>

#include <windows.h>
#include <fileapi.h>
#include <synchapi.h>

using namespace std;

const unsigned N = 100000000;
const unsigned blockSize = 60;			// 03050(6)
const unsigned threadsCount = 16;		// 1, 2, 4, 8, 12, 16

void calculatorNumberPi(void* parametersStart);

int main() {

	system("chcp 437");
	system("cls");
	cout << "This is a console application by Artem Ivanov,\n";
	cout << "a student of group 0305, for laboratory work\n";
	cout << "on the study of processor management using the Win32 API.\n\n";
	
	int start, finish;

	// array of parameters are taking by each thread

	HANDLE threads[threadsCount];
	void* params[threadsCount][3];
	int startingOperations[threadsCount];
	HANDLE mutex = CreateMutex(NULL, FALSE, NULL);
	double pi = 0;


	for (int i = 0; i < threadsCount; ++i) {
		startingOperations[i] = i * blockSize;
		params[i][0] = &pi;
		params[i][1] = startingOperations + i;
		params[i][2] = &mutex;

		threads[i] = CreateThread(
			NULL,					// default security attributes	
			0,						// use default stack size  
			(LPTHREAD_START_ROUTINE)
			calculatorNumberPi,		// thread function name
			params[i],				// arguments for the threading function
			CREATE_SUSPENDED,		// create suspended (explanation below)
			NULL					// returns the thread identifier
		);
							//explanation
		// threads are suspended because it takes time to create a thread, 
		// during which previously created threads will be executed, 
		// which reduces the accuracy of the TIME measurement. 
	}

	start = GetTickCount();
	for (int i = 0; i < threadsCount; ++i)
		ResumeThread(threads[i]);

	WaitForMultipleObjects(threadsCount, threads, TRUE, INFINITE);
	finish = GetTickCount();

	cout.precision(N);
	int time = finish - start;
	cout << "Pi - " << pi << endl << "Time - " << time << " ms." << endl;

	// close handles of threads
	for (int i = 0; i < threadsCount; ++i)
		CloseHandle(threads[i]);
	CloseHandle(mutex);

	return 0;
}

void calculatorNumberPi(void* parametersStart) {
	// convert input variable to array of void* pointers in order to get params
	void** parameters = (void**)parametersStart;

	// get params from input array
	double* piPart = (double*)(parameters[0]);					// ... - address of calculated pieces of num pi
	unsigned start_block_adress = *(unsigned*)(parameters[1]);	// ... - determines the location of the blocks

	HANDLE mutex = *(HANDLE*)(parameters[2]);

	double keepingPi = 0;

	// while we are not put of pi bounds
	while (start_block_adress < N) {
		// calculate block using given formula
		for (int i = start_block_adress; i < start_block_adress + blockSize; ++i)
			keepingPi += 4 / (1 + ((i + 0.5) / N) * ((i + 0.5) / N));

		// go to next block
		start_block_adress += threadsCount * blockSize;

	}

	WaitForSingleObject(mutex, INFINITE);
	// an answer is a sum of all parts calculated by threads
	*piPart += keepingPi / N;
	ReleaseMutex(mutex);
}