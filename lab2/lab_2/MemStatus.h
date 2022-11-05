#pragma once

#include <iostream>
#include <windows.h>
#include <fileapi.h>

using namespace std;

void MemStatus() {
    
    // memory info getting
    LPMEMORYSTATUSEX  status;
    status = new MEMORYSTATUSEX();
    status->dwLength = sizeof(*status);
    GlobalMemoryStatusEx(status);
    // memory info printing
    cout << "\nUsed " << status->dwMemoryLoad << "% of memory\n";
    cout << "Total bytes of physical memory: " << status->ullTotalPhys << "\n";
    cout << "Free bytes of physical memory: " << status->ullAvailPhys << "\n";
    cout << "Total bytes of paging file: " << status->ullTotalPageFile << "\n";
    cout << "Free bytes of paging file: " << status->ullAvailPageFile << "\n";
    cout << "Total bytes of virtual memory: " << status->ullTotalVirtual << "\n";
    cout << "Free bytes of virtual memory: " << status->ullAvailVirtual << "\n";
    
    delete status;
}