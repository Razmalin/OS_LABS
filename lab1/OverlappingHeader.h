#pragma once

#include <iostream>
#include <windows.h>
#include <fileapi.h>
#include "DirectoryHeader.h"
#include "FileHeader.h"

using namespace std;


// handles on files involved in the copy operation
HANDLE firstHandle, secondHandle;
int callback = 0;
// Completion function that will be called whenever an I/O operation is completed (for ReadFileEx and WriteFileEx)
VOID CALLBACK FileIOCompletionRoutine(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped) {
    // number of callbacks
    callback++;
}
void ReadFileOverlapped(long long, DWORD, int, OVERLAPPED*, char**);
void WriteFileOverlapped(long long, DWORD, int, OVERLAPPED*, char**);
// Asynchronous copy function
void copyFile(DWORD, int);

void Overlapping() {
    int part = 0, operations = 0, sizeToCopy;
    wchar_t fileName[256], filePath[256];
    DWORD startTime;
    DWORD endTime;
    //Entering a multiplier for the block size
    cout << "Enter the read-write block size (in bytes):\n";
    do {
        cout << "> 4096*";
        part = num_cin();
        if (part <= 0) cout << "Try again!\n";
    } while (part <= 0);
    //Entering the number of operations
    cout << "Enter number of operations(2^n = ENTER): ";
    do {
        operations = num_cin();
        if (operations <= 0) cout << "Try again!\n";
    } while (operations <= 0);

    cout << "Enter the path of copied file (dir\\test.txt for example): ";
    do {
        //checks by both absolute and relative paths
        wcin >> fileName;
        if (!isFile(fileName))
            cout << "This file doesn't exist\nTry again\n";
    } while (!isFile(fileName));
    cout << "Enter the path of copy (dir\\test.txt for example): ";
    wcin >> filePath;

    if (wcslen(fileName) < MAX_PATH && wcslen(filePath) < MAX_PATH) {
        
        //FILE_FLAG_NO_BUFFERING & FILE_FLAG_OVERLAPPED
        
        firstHandle = CreateFile(fileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED, NULL);
        secondHandle = CreateFile(filePath, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED, NULL);

        if (firstHandle != INVALID_HANDLE_VALUE) {
            sizeToCopy = 4096 * part;
            startTime = GetTickCount();
            copyFile(sizeToCopy, operations);
            endTime = GetTickCount();
            cout << "Copying took " << endTime - startTime << " milliseconds\n";
        }
        else cout << "Error 0x" << GetLastError() << endl;
    }

    if (!CloseHandle(firstHandle)) cout << "Unable to close first file\n";
    if (!CloseHandle(secondHandle))cout << "Unable to close second file\n";
}

void copyFile(DWORD blockSize, int count) {
    DWORD high = 0;
    long long fileSize = GetFileSize(firstHandle, &high),
        curSize = fileSize;
    char** buffer = new char* [count];
    OVERLAPPED* over_1 = new OVERLAPPED[count],
        * over_2 = new OVERLAPPED[count];

    for (int i = 0; i < count; i++) {
        buffer[i] = new char[blockSize];

        over_1[i].Offset = over_2[i].Offset = i * blockSize;
        over_1[i].OffsetHigh = over_2[i].OffsetHigh = i * high;
        over_1[i].hEvent = over_2[i].hEvent = NULL;
    }
    do {
        ReadFileOverlapped(fileSize, blockSize, count, over_1, buffer);
        WriteFileOverlapped(fileSize, blockSize, count, over_2, buffer);
        curSize -= (long long)(blockSize * count);
    } while (curSize > 0);

    SetFilePointer(secondHandle, fileSize, NULL, FILE_BEGIN);
    SetEndOfFile(secondHandle);

    for (int i = 0; i < count; i++)
        delete[] buffer[i];
    delete[] buffer;
    delete[] over_1;
    delete[] over_2;
}

void ReadFileOverlapped(long long fileSize, DWORD blockSize, int operationsCount, OVERLAPPED* overlappeds, char** buffer) {
    int operations_counter = 0;
    for (int i = 0; i < operationsCount; i++)
        if (fileSize > 0) {
            operations_counter++;
            ReadFileEx(firstHandle, buffer[i], blockSize, &overlappeds[i], FileIOCompletionRoutine);
            fileSize -= blockSize;
        }
    while (callback < operations_counter)
        SleepEx(-1, true);
    for (int i = 0; i < operationsCount; i++)
        overlappeds[i].Offset += blockSize * operationsCount;
    callback = 0;
}

void WriteFileOverlapped(long long fileSize, DWORD blockSize, int operationsCount, OVERLAPPED* overlappeds, char** buffer) {
    int operations_counter = 0;
    for (int i = 0; i < operationsCount; i++)
        if (fileSize > 0) {
            operations_counter++;
            WriteFileEx(secondHandle, buffer[i], blockSize, &overlappeds[i], FileIOCompletionRoutine);
            fileSize -= blockSize;
        }
    while (callback < operations_counter)
        SleepEx(-1, true);
    for (int i = 0; i < operationsCount; i++)
        overlappeds[i].Offset += blockSize * operationsCount;
    callback = 0;
}