#pragma once

#include <iostream>

#include <windows.h>
#include <fileapi.h>
#include "DirectoryHeader.h"

using namespace std;

	//input_time() user input data to SYSTEMTIME struct

void input_time(SYSTEMTIME* time) {
	WORD value;

	cout << "Enter the time (UTC + 0).\n";
	cout << "Enter a year: ";
	wcin >> value;
	wcin.ignore(INT_MAX, '\n');
	time->wYear = value;
	cout << "Enter a month: ";
	wcin >> value;
	wcin.ignore(INT_MAX, '\n');
	time->wMonth = value;
	cout << "Enter a day: ";
	wcin >> value;
	wcin.ignore(INT_MAX, '\n');
	time->wDay = value;
	cout << "Enter an hour: ";
	wcin >> value;
	wcin.ignore(INT_MAX, '\n');
	time->wHour = value;
	cout << "Enter a minute: ";
	wcin >> value;
	wcin.ignore(INT_MAX, '\n');
	time->wMinute = value;
	cout << "Enter a second: ";
	wcin >> value;
	wcin.ignore(INT_MAX, '\n');
	time->wSecond = value;
}

//prints all file attribute flags that DWORD contains

void FlagsDecoder(DWORD flags) {
	if (flags & FILE_ATTRIBUTE_ARCHIVE) cout << " - File is an archive\n";
	if (flags & FILE_ATTRIBUTE_COMPRESSED) cout << " - File is compressed\n";
	if (flags & FILE_ATTRIBUTE_DEVICE) cout << " - File is reserved for system use\n";
	if (flags & FILE_ATTRIBUTE_DIRECTORY) cout << " - File is a directory\n";
	if (flags & FILE_ATTRIBUTE_ENCRYPTED) cout << " - File is encrypted\n";
	if (flags & FILE_ATTRIBUTE_HIDDEN) cout << " - File is hidden\n";
	if (flags & FILE_ATTRIBUTE_INTEGRITY_STREAM) cout << " - File is configured with integrity\n";
	if (flags & FILE_ATTRIBUTE_NORMAL) cout << " - File has no special attributes\n";
	if (flags & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED) cout << " - File not to be indexed by the content indexing service\n";
	if (flags & FILE_ATTRIBUTE_NO_SCRUB_DATA) cout << " - File is not to be read by the background data integrity scanner\n";
	if (flags & FILE_ATTRIBUTE_OFFLINE) cout << " - File is not available immediately\n";
	if (flags & FILE_ATTRIBUTE_READONLY) cout << " - File is readonly\n";
	if (flags & FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS) cout << " - File is not fully present locally\n";
	if (flags & FILE_ATTRIBUTE_RECALL_ON_OPEN) cout << " - File is virtual\n";
	if (flags & FILE_ATTRIBUTE_REPARSE_POINT)cout << " - File is a symbolic link\n";
	if (flags & FILE_ATTRIBUTE_SPARSE_FILE) cout << " - File is sparse\n";
	if (flags & FILE_ATTRIBUTE_SYSTEM) cout << " - System uses only a part of this file\n";
	if (flags & FILE_ATTRIBUTE_TEMPORARY) cout << " - File is being used for temporary storage\n";
	if (flags & FILE_ATTRIBUTE_VIRTUAL) cout << " - File is reserved for system use\n";
	if (flags & FILE_ATTRIBUTE_PINNED) cout << " - File should be kept fully present locally even when not being actively accessed\n";
	if (flags & FILE_ATTRIBUTE_UNPINNED) cout << " - File should not be kept fully present locally except when being actively accessed\n";
}

void FileAttributes() {

	HANDLE file;
	FILETIME file_time;
	SYSTEMTIME time;
	int option = -1;
	wchar_t path[256];

	DWORD new_flags;
	_BY_HANDLE_FILE_INFORMATION File_info;
	//opening file
	cout << "Enter the full path of file (D:\\Users\\User\\file.txt for example): ";
	wcin >> path;
	file = CreateFile(path, GENERIC_READ | FILE_WRITE_ATTRIBUTES, FILE_SHARE_DELETE | FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == INVALID_HANDLE_VALUE) {
		cout << "File does not exist." << endl;
		return;
	}

	// getting information
	if (!GetFileInformationByHandle(file, &File_info)) {
		cout << "Error of getting information about a file." << endl;
		return;
	}

	// decoder of flags
	FlagsDecoder(File_info.dwFileAttributes);

	// time sharing
	FileTimeToSystemTime(&(File_info.ftCreationTime), &time);
	cout << " - File was created at: " << time.wDay << "." << time.wMonth << "." << time.wYear
		<< ", " << time.wHour << "h : " << time.wMinute << "m : " << time.wSecond << "s UTC 0,\n";
	FileTimeToSystemTime(&(File_info.ftLastAccessTime), &time);
	cout << " - File was last accessed at: " << time.wDay << "." << time.wMonth << "." << time.wYear
		<< ", " << time.wHour << "h : " << time.wMinute << "m : " << time.wSecond << "s UTC 0,\n";
	FileTimeToSystemTime(&(File_info.ftLastWriteTime), &time);
	cout << " - File was last changed at: " << time.wDay << "." << time.wMonth << "." << time.wYear
		<< ", " << time.wHour << "h : " << time.wMinute << "m : " << time.wSecond << "s UTC 0,\n";

	// size info
	cout << "\n - File size: " << File_info.nFileSizeHigh << File_info.nFileSizeLow;
	cout << " bytes" << endl;

	// changing flags
	cout << "\nDo you want to change file flags?";
	cout << "\n '1' - YES";
	cout << "\n '2' - NO\n";
	cout << "Please select an option: ";
	option = num_cin();
	cout << endl;
	if (option == 1) {
		cout << "Enter the word of flags: ";
		wcin >> new_flags;
		if (SetFileAttributes(path, new_flags))
			cout << "Flags are set.\n";
		else
			cout << "Failed to set flags.\n";
	}

	// changing time
	cout << "Do you want to change file creation time?";
	cout << "\n '1' - YES";
	cout << "\n '2' - NO\n";
	cout << "Please select an option: ";
	option = num_cin();
	cout << endl;
	if (option == 1) {
		input_time(&time);
		SystemTimeToFileTime(&time, &file_time);
		if (SetFileTime(file, &file_time, NULL, NULL))
			cout << "Time is set successfully.\n";
		else
			cout << "Error of setting time.\n";
	}

	cout << "Do you want to change file last access time?";
	cout << "\n '1' - YES";
	cout << "\n '2' - NO\n";
	cout << "Please select an option: ";
	option = num_cin();
	cout << endl;
	if (option == 1) {
		input_time(&time);
		SystemTimeToFileTime(&time, &file_time);
		if (SetFileTime(file, NULL, &file_time, NULL))
			cout << "Time is set successfully.\n";
		else
			cout << "Error of setting time.\n";
	}

	cout << "Do you want to change file last change time?";
	cout << "\n '1' - YES";
	cout << "\n '2' - NO\n";
	cout << "Please select an option: ";
	option = num_cin();
	cout << endl;
	if (option == 1) {
		input_time(&time);
		SystemTimeToFileTime(&time, &file_time);
		if (SetFileTime(file, NULL, NULL, &file_time))
			cout << "Time is set successfully.\n";
		else
			cout << "Error of setting time.\n";
	}

	CloseHandle(file);
}