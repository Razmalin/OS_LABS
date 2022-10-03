#pragma once

#include <iostream>

#include <windows.h>
#include <fileapi.h>

using namespace std;

bool isFile(LPCWSTR strFileName) {
    DWORD ret = GetFileAttributes(strFileName);
    return ((ret != INVALID_FILE_ATTRIBUTES) && !(ret & FILE_ATTRIBUTE_DIRECTORY));
}

void createFile() {
    wchar_t path[256], temp[50];
    HANDLE handle;

    fflush(stdout);
    cout << "Enter the full name of file (D:\\Users\\User\\file.txt for example): ";
    wcin >> path;
    fflush(stdout);
    wcscpy(temp, path);

    if (path[0] == '\\') {
        wchar_t* buffer = new wchar_t[256];
        GetCurrentDirectory(256, buffer);
        wcscat(buffer, path);
        wcscpy(path, buffer);
        delete[] buffer;
    }

    handle = CreateFile(path, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (handle == INVALID_HANDLE_VALUE) printf("ERROR 0x%x \nFile not created\n", GetLastError());
    else cout << "File " << temp << " created\n";
    CloseHandle(handle);
}

void copyAndReplaceFiles() {
    wchar_t fileName[256], fileNameAfter[256];
    int option = -1;
    while (option != 1 && option != 2) {
        cout << "What do you want?\n";
        cout << "1 - copy file\n";
        cout << "2 - move file\n";
        cout << "Enter your choice: ";
        option = num_cin();
        if (option != 1 && option != 2)
            cout << "Wrong option!\n";
    }

    cout << "Enter the full path of file (D:\\Users\\User\\file.txt for example):  ";
    do {
        wcin >> fileName;
        if (!isFile(fileName))
            cout << "This file doesn't exist\nTry again\n";
    } while (!isFile(fileName));
    cout << "Enter the new full path of file (D:\\Users\\User\\newfile.txt for example): ";
    wcin >> fileNameAfter;
    if (option == 1) {
        if (CopyFile(fileName, fileNameAfter, true))
            cout << "The file has been copied successfully." << endl;
        else
            cout << "Error. Did you enter correct paths?" << endl;
    }
    else {
        if (MoveFile(fileName, fileNameAfter))
            cout << "The file has been moved successfully." << endl;
        else
            cout << "Error. Did you enter correct paths?" << endl;
    }

}