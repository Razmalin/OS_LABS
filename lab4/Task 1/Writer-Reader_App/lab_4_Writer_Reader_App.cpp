#include <iostream>
#include <windows.h>

using namespace std;

int main() {
	const unsigned page_size = 4096; //bytes
	const unsigned page_number = 0 + 3 + 0 + 5 + 0 + 6;

	HANDLE reading_semaphores[page_number];
	HANDLE writing_semaphores[page_number];
	HANDLE process_handles[page_number * 2];
	HANDLE shared_file;
	HANDLE shared_file_mapping_object;
	PROCESS_INFORMATION reader_info[page_number];
	PROCESS_INFORMATION writer_info[page_number];
	STARTUPINFO startup_info;
	SECURITY_ATTRIBUTES semaphore_security_attributes = { sizeof(semaphore_security_attributes), NULL, TRUE };
	wchar_t buffer[50] = L"";
	wchar_t reading_semaphore_name[page_number][15];
	wchar_t	writing_semaphore_name[page_number][15];

	// INIT SHARED OBJECT
	shared_file = CreateFile(L"sharedfile.txt",
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	shared_file_mapping_object = CreateFileMapping(shared_file,
		NULL,
		PAGE_READWRITE,
		0,
		page_size * page_number,
		L"Local\\SharedFile"
	);

	// GET STARTUP INFO FOR NEW PROCESSES
	ZeroMemory(&startup_info, sizeof(startup_info));
	startup_info.cb = sizeof(startup_info);
	
	// INIT SEMAPHORES
	for (int i = 0; i < page_number; ++i) {
		wcscpy_s(reading_semaphore_name[i], L"Local\\rSemX");
		reading_semaphore_name[i][10] = '0' + i;
		wcscpy_s(writing_semaphore_name[i], L"Local\\wSemX");
		writing_semaphore_name[i][10] = '0' + i;

		reading_semaphores[i] = CreateSemaphore(&semaphore_security_attributes, 0, 1, reading_semaphore_name[i]);
		writing_semaphores[i] = CreateSemaphore(&semaphore_security_attributes, 1, 1, writing_semaphore_name[i]);

		ZeroMemory(reader_info + i, sizeof(*reader_info));
		ZeroMemory(writer_info + i, sizeof(*writer_info));
	}
	const wchar_t PATH_Reader[] = L"C:\\Users\\artem\\source\\repos\\lab_4_Reader\\Debug\\lab_4_Reader.exe";
	const wchar_t PATH_Writer[] = L"C:\\Users\\artem\\source\\repos\\lab_4_Writer\\Debug\\lab_4_Writer.exe";

	// PROCESSES INIT
	for (int i = 0; i < page_number; ++i) {
		CreateProcess(PATH_Reader,
			buffer,
			NULL,
			NULL,
			TRUE,
			CREATE_SUSPENDED,
			NULL,
			NULL,
			&startup_info,
			reader_info + i
		);
		CreateProcess(PATH_Writer,
			buffer,
			NULL,
			NULL,
			TRUE,
			CREATE_SUSPENDED,
			NULL,
			NULL,
			&startup_info,
			writer_info + i
		);

		process_handles[i] = (reader_info + i)->hProcess;
		process_handles[page_number * 2 - 1 - i] = (writer_info + i)->hProcess;
	}

	// PROCESSES STARTUP
	for (int i = 0; i < page_number; ++i) {
		ResumeThread((reader_info + i)->hThread);
		ResumeThread((writer_info + i)->hThread);
	}

	// WAIT FOR PROCESSES END
	WaitForMultipleObjects(page_number * 2, process_handles, TRUE, INFINITE);

	// END OF PROGRAM
	for (int i = 0; i < page_number; ++i) {
		CloseHandle((reader_info + i)->hProcess);
		CloseHandle((reader_info + i)->hThread);

		CloseHandle((writer_info + i)->hProcess);
		CloseHandle((writer_info + i)->hThread);

		CloseHandle(reading_semaphores[i]);
		CloseHandle(writing_semaphores[i]);
	}
	CloseHandle(shared_file);
	CloseHandle(shared_file_mapping_object);

	return 0;
}