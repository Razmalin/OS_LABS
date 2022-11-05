#pragma once

#include <iostream>
#include <windows.h>
#include <fileapi.h>
#include "choise.h"

using namespace std;

/*
	what_OEM() prints the processor architecture 
*/

void what_OEM(DWORD arch_info) {
	switch (arch_info) {
	case PROCESSOR_ARCHITECTURE_AMD64:
		cout << "\nProcessor architecture: AMD64\n";
		break;
	case PROCESSOR_ARCHITECTURE_ARM:
		cout << "\nProcessor architecture: ARM\n";
		break;
	case PROCESSOR_ARCHITECTURE_ARM64:
		cout << "\nProcessor architecture: ARM64\n";
		break;
	case PROCESSOR_ARCHITECTURE_IA64:
		cout << "\nProcessor architecture: Intel Itanium-based\n";
		break;
	case PROCESSOR_ARCHITECTURE_INTEL:
		cout << "\nProcessor architecture: x86\n";
		break;
	case PROCESSOR_ARCHITECTURE_UNKNOWN:
		cout << "\nProcessor architecture: unknown\n";
		break;
	default:
		cout << "\nProcessor architecture: UNEXPECTED VALUE\n";
	}
}

/*
	SysInfo() getting system info & printing system info
*/

void SysInfo() {
	SYSTEM_INFO SysInfo;

	GetNativeSystemInfo(&SysInfo);
	what_OEM(SysInfo.wProcessorArchitecture);

	cout << "Page size: " << SysInfo.dwPageSize << endl;
	cout << "Minimum memory application address: " << SysInfo.lpMinimumApplicationAddress << endl;
	cout << "Miximum memory application address: " << SysInfo.lpMaximumApplicationAddress << endl;
	cout << "Active processor mask: " << SysInfo.dwActiveProcessorMask << endl;
	cout << "Number of processors: " << SysInfo.dwNumberOfProcessors << endl;
	cout << "Processor type: " << SysInfo.dwProcessorType << endl;
	cout << "Allocation granularity: " << SysInfo.dwAllocationGranularity << endl;
	cout << "Processor level: " << SysInfo.wProcessorLevel << endl;
	cout << "Processor revision: " << SysInfo.wProcessorRevision << endl;
}