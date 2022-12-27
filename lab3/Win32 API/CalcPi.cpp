#include <iostream>
#include <windows.h>
#include <time.h>

using namespace std;

const int N = 100000000;
const int blockSize = 60;            // 03050(6)
const int threadsCount = 16;         // 1, 2, 4, 8, 12, 16
const int TIMES = 10;                // 10 times do test for average result
int currentPos = 0;
double pi = 0.0;

LPCRITICAL_SECTION section = new CRITICAL_SECTION;

DWORD WINAPI calculatorNumberPi(LPVOID lpParam);

int main() {
    system("chcp 437");
    system("cls");
    cout << "This is a console application by Artem Ivanov,\n";
    cout << "a student of group 0305, for laboratory work\n";
    cout << "on the study of processor management using the Win32 API.\n\n";

    DWORD start_time;
    HANDLE threads[threadsCount];
    int position[threadsCount];
    double time = 0.0;
    for (int j = 0; j < TIMES; j++) {
        pi = 0.0;
        InitializeCriticalSection(section);

        for (int i = 0; i < threadsCount; ++i) {
            position[i] = blockSize * i;
            currentPos = position[i];
            threads[i] = CreateThread(
                NULL,               // default security attributes
                0,                  // use default stack size  
                calculatorNumberPi,   // thread function name
                &position[i],       // arguments for threaded function
                CREATE_SUSPENDED,   // created stopped (explanation below)
                NULL);              // returns the thread identifier
        }

                                //explanation
        // threads are suspended because it takes time to create a thread, 
        // during which previously created threads will be executed, 
        // which reduces the accuracy of the TIME measurement.

        start_time = clock();

        for (int i = 0; i < threadsCount; ++i)
            ResumeThread(threads[i]);

        WaitForMultipleObjects(threadsCount, threads, TRUE, INFINITE);

        pi /= (long double)N;

        DeleteCriticalSection(section);

        time += clock() - start_time;

        // close handles of threads
        for (int i = 0; i < threadsCount; ++i)
            CloseHandle(threads[i]);
    }
    printf("Pi -  %.10lf\n", pi);
    cout << endl << "Time - " << (long double)(time / TIMES) / CLOCKS_PER_SEC << " ms." << endl;
}

DWORD WINAPI calculatorNumberPi(LPVOID lpParam) {
    int* start_block_adress = (int*)lpParam;
    int end = *start_block_adress + blockSize;
    long double x, keepingPi;
    while (*start_block_adress < N) {
        keepingPi = 0.0;
        // calculate block using given formula
        for (int i = *start_block_adress; (i < end) && (i < N); ++i) {
            x = (i + 0.5) / N;
            keepingPi += (4 / (1 + x * x));
        }
        EnterCriticalSection(section);
        // an answer is a sum of all parts calculated by threads
        pi += keepingPi;
        currentPos += blockSize;
        *start_block_adress = currentPos;
        LeaveCriticalSection(section);
        end = *start_block_adress + blockSize;
    }
    return 0;
}
