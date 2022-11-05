#pragma once

#include <iostream>
#include <windows.h>
#include <fileapi.h>

using namespace std;

void MemState() {
	MEMORY_BASIC_INFORMATION info;
	PVOID address = nullptr;


	// address of region
	cout << "Enter the address of the virtual memory that you want to get information about (hexadecimal): ";
	wcin >> std::hex >> address >> std::dec;

	if (address == nullptr) {
		cout << "Failed to analyze the address.\n";
		return;
	}

	// getting info
	VirtualQuery(address, &info, sizeof(info));

	// printing info
	cout << "\nBase address of the region: " << info.BaseAddress << "\n";
	cout << "Identified base address of the region: " << info.AllocationBase << "\n";
	cout << "The security attribute assigned to the region when reserving it (hexadecimal): " << std::hex << info.AllocationProtect << std::endl << " \n";
	cout << "Current region protection parameter (hexadecimal): " << std::hex << info.Protect << std::endl;
	cout << "Size of the region (decimal): " << std::dec << info.RegionSize << std::endl;
	
	/*STATE reports the status (MEM_FREE, MEM_RESERVE, or MEM_COMMIT) 
	of all adjacent pages that have the same security attributes, 
	status, and type as the page located at the address 
	specified in the lpAddress parameter*/
	
	switch (info.State) {
	case MEM_COMMIT:
		cout << "Physical storage was allocated for this region ('MEM_COMMIT')\n";
		break;
	case MEM_FREE:
		cout << "This memory was not reserved neither has not been target of commit ('MEM_FREE')\n";
		break;
	case MEM_RESERVE:
		cout << "Region is reserved with no physical storage ('MEM_RESERVE')\n";
		break;
	default:
		cout << "Memory state: ERROR\n";
	}

	/*TYPE identifies the type of physical memory (MEM_IMAGE, MEM_MAPPED, or MEM_PRIVATE) 
	associated with a group of adjacent pages that have the same security attributes, 
	state, and type as the page located at the address specified in the lpAddress parameter*/

	switch (info.Type) {
	case MEM_IMAGE:
		cout << "Memory pages within the region are mapped into the view of an image section ('MEM_IMAGE')\n";
		break;
	case MEM_MAPPED:
		cout << "Memory pages within the region are mapped into the view of a section ('MEM_MAPPED')\n";
		break;
	case MEM_PRIVATE:
		cout << "Memory pages within the region are private ('MEM_PRIVATE')\n";
		break;
	default:
		cout << "Memory type: ERROR\n";
	}
}