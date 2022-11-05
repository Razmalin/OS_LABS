#pragma once

#include <iostream>
#include <windows.h>
#include <fileapi.h>

using namespace std;

void FreeMem() {
    LPVOID address = NULL;
    cout << "Enter the address: ";
    cin >> std::hex >> address;

    if (VirtualFree(address, 0, MEM_RELEASE))
        cout << "Memory area's released\n";
    else cerr << "Error: 0x" << GetLastError() << endl;
}
