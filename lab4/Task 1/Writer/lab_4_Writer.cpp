#include <fstream>
#include <windows.h>

#define ITERATIONS_N 10
using namespace std;

void log(const char* message, int page_no, ostream& logfile);

int main() {
	const unsigned page_size = 4096; //bytes
	const unsigned page_number = 0 + 3 + 0 + 5 + 0 + 6;

	HANDLE reading_semaphores[page_number];
	HANDLE writing_semaphores[page_number];
	HANDLE shared_file_mapping_object;
	ofstream logfile;
	void* pages[page_number];
	void* view_of_file;
	wchar_t semaphore_name_r[page_number][15];
	wchar_t semaphore_name_w[page_number][15];
	wchar_t logfile_name[50] = L"log\\writers\\log_";
	wchar_t process_id[10];
	int wait_time_in_ms;
	int free_page_index;
	int semaphore_acquisition_result;

	// INIT LOG
	_itow_s(GetCurrentProcessId(), process_id, 10);
	wcscat_s(logfile_name, process_id);
	wcscat_s(logfile_name, L".txt");
	logfile.open(logfile_name, ofstream::out);

	// INIT VIEW OF FILE
	shared_file_mapping_object = OpenFileMapping(FILE_MAP_WRITE, FALSE, L"Local\\SharedFile");

	view_of_file = MapViewOfFile(shared_file_mapping_object, FILE_MAP_WRITE, 0, 0, page_number * page_size);
	VirtualLock(view_of_file, page_number * page_size);

	// INIT SEMAPHORES
	for (int i = 0; i < page_number; ++i) {
		wcscpy_s(semaphore_name_r[i], L"Local\\rSemX");
		semaphore_name_r[i][10] = '0' + i;
		wcscpy_s(semaphore_name_w[i], L"Local\\wSemX");
		semaphore_name_w[i][10] = '0' + i;

		reading_semaphores[i] = OpenSemaphore(SEMAPHORE_MODIFY_STATE, FALSE, semaphore_name_r[i]);
		writing_semaphores[i] = OpenSemaphore(SYNCHRONIZE, FALSE, semaphore_name_w[i]);

		pages[i] = (byte*)view_of_file + i * page_size;
	}

	// START WRITING
	for (int i = 0; i < ITERATIONS_N; ++i) {
		log("Started waiting", -1, logfile);
		free_page_index = WaitForMultipleObjects(page_number, writing_semaphores, FALSE, INFINITE) - WAIT_OBJECT_0;
		log("Acquired", free_page_index, logfile);

		if (free_page_index >= 0 && free_page_index < page_number) {
			wait_time_in_ms = rand() % 1001 + 500;
			log("Started reading", free_page_index, logfile);
			Sleep(wait_time_in_ms);
			log("Ended reading", free_page_index, logfile);
			ReleaseSemaphore(reading_semaphores[free_page_index], 1, NULL);
			log("Released", free_page_index, logfile);
		}
	}

	// CLOSE HANDLES
	for (int i = 0; i < page_number; ++i) {
		CloseHandle(reading_semaphores[i]);
		CloseHandle(writing_semaphores[i]);
	}
	VirtualUnlock(view_of_file, page_size);
	UnmapViewOfFile(view_of_file);
	logfile.close();

	return 0;
}

void log(const char* message, int page_no, ostream& logfile) {
	logfile << message << " at " << GetTickCount64();
	if (page_no > 0)
		logfile << ", page number " << page_no;
	logfile << "\n";
}