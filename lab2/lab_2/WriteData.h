#pragma once

#include <iostream>
#include <windows.h>
#include <fileapi.h>
#include "choise.h"
#include <string>

using namespace std;

void WriteData() {
	MEMORY_BASIC_INFORMATION info;
	PVOID address = nullptr;

	// specify region
	cout << "Enter the address belonging to region you want to fill (hexadecimal): ";
	wcin >> std::hex >> address >> std::dec;
	wcin.ignore(INT_MAX, '\n');
	if (address == nullptr) {
		cout << "Failed to parse address.\n";
		return;
	}

	// get info about region
	VirtualQuery(address, &info, sizeof(info));

	// check for mistakes at getting info
	if (GetLastError() != 0) {
		cout << "Failed. Error " << GetLastError() << ".\n";
		SetLastError(0);
		return;
	}

	// check if memory is accessable
	if (info.State != MEM_COMMIT) {
		cout << "Failed. Memory is not allocated.\n";
		return;
	}

	// write data
	string source;
	cout << "Enter data for input: ";
	(void)getchar();
	getline(cin, source);

	// show typed info
	char* destination = (char*)address;
	CopyMemory(destination, source.c_str(), source.length() * sizeof(char));
	cout << "Memory area " << address << " filled. Entered data: ";
	for (size_t i = 0; i < source.length(); i++)
		cout << destination[i];
	cout << endl;

	

}