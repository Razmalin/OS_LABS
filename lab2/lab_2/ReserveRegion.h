#pragma once

#include <iostream>
#include <windows.h>
#include <fileapi.h>
#include "choise.h"

using namespace std;

void ReserveRegion()
{
	PVOID address = nullptr;
	SIZE_T regionsize = 4096;
	int choise;

	// get desired address if required
	cout << "If you want to specify a reservation address, enter 1, otherwise enter any number: ";
	choise = num_cin();
	if (choise == 1) {
		cout << "Enter the start of region you want to reserve (hexadecimal): ";
		wcin >> std::hex >> address >> std::dec;
		wcin.ignore(INT_MAX, '\n');

		cout << "Enter the size of region you want to reserve (decimal): ";
		wcin >> regionsize;
		wcin.ignore(INT_MAX, '\n');
	}
	else
		cout << "Address will be chosen automatically."
		<< "\nSize of region will be 4096 bytes.\n";

	address = VirtualAlloc(address, regionsize, MEM_RESERVE, PAGE_READWRITE);
	if (address != NULL)
	{
		cout << "Allocation succseffsul" << endl << "Address - " << std::dec << address << ".\n";;
	}
	else
		cout << "Allocation and commit failed. Error " << GetLastError() << ".\n";
	SetLastError(0);
}