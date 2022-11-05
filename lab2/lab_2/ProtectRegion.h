#pragma once
#include <iostream>
#include <windows.h>
#include <fileapi.h>
#include "choise.h"
#include <string>

using namespace std;

void choose_protection(DWORD& newLevel) {
    newLevel = -1;
    while (newLevel < 1 || newLevel > 10) {
        //cout << "Enter the memory protection:\n";
        cout << "1 - PAGE_EXECUTE\n";
        cout << "2 - PAGE_EXECUTE_READ\n";
        cout << "3 - PAGE_EXECUTE_READWRITE\n";
        cout << "4 - PAGE_EXECUTE_WRITECOPY\n";
        cout << "5 - PAGE_NOACCESS\n";
        cout << "6 - PAGE_READONLY\n";
        cout << "7 - PAGE_READWRITE\n";
        cout << "8 - PAGE_WRITECOPY\n";
        cout << "9 - PAGE_TARGETS_INVALID\n";
        cout << "10 - PAGE_TARGETS_NO_UPDATE\n";
        cout << "Enter your choice: ";
        newLevel = num_cin();
        if (newLevel < 1 || newLevel > 10) cout << "Wrong option\n";
    }

    if (newLevel == 1) newLevel = PAGE_EXECUTE;
    else if (newLevel == 2) newLevel = PAGE_EXECUTE_READ;
    else if (newLevel == 3) newLevel = PAGE_EXECUTE_READWRITE;
    else if (newLevel == 4) newLevel = PAGE_EXECUTE_WRITECOPY;
    else if (newLevel == 5) newLevel = PAGE_NOACCESS;
    else if (newLevel == 6) newLevel = PAGE_READONLY;
    else if (newLevel == 7) newLevel = PAGE_READWRITE;
    else if (newLevel == 8) newLevel = PAGE_WRITECOPY;
    else if (newLevel == 9) newLevel = PAGE_TARGETS_INVALID;
    else newLevel = PAGE_TARGETS_NO_UPDATE;
}

void protect_info(const DWORD& pro) {
    if (pro & PAGE_EXECUTE) cout << "PAGE_EXECUTE\n";
    if (pro & PAGE_EXECUTE_READ) cout << "PAGE_EXECUTE_READ\n";
    if (pro & PAGE_EXECUTE_READWRITE) cout << "PAGE_EXECUTE_READWRITE\n";
    if (pro & PAGE_EXECUTE_WRITECOPY) cout << "PAGE_EXECUTE_WRITECOPY\n";
    if (pro & PAGE_NOACCESS) cout << "PAGE_NOACCESS\n";
    if (pro & PAGE_READONLY) cout << "PAGE_READONLY\n";
    if (pro & PAGE_READWRITE) cout << "PAGE_READWRITE\n";
    if (pro & PAGE_WRITECOPY) cout << "PAGE_WRITECOPY\n";
    if (pro & PAGE_TARGETS_INVALID) cout << "PAGE_TARGETS_INVALID\n";
    if (pro & PAGE_TARGETS_NO_UPDATE) cout << "PAGE_TARGETS_NO_UPDATE\n";
}

void ProtectRegion() {
    LPVOID address;
    DWORD newLevel, oldLevel;

    cout << "Enter the address belonging to region you want to fill (hexadecimal): ";
    wcin >> std::hex >> address >> std::dec;
    wcin.ignore(INT_MAX, '\n');
    if (address == nullptr) {
        cout << "Failed to parse address.\n";
        return;
    }
    cout << "Choose new protection level:\n";
    choose_protection(newLevel);

    if (VirtualProtect(address, sizeof(DWORD), newLevel, &oldLevel)) {
        cout << "Old protection level: ";
        protect_info(oldLevel);
        cout << "New protection level: ";
        protect_info(newLevel);
    }
    else cout << "Error " << GetLastError() << "\nAccess denied\n";
}

