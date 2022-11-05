#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <windows.h>

using namespace std;

int num_cin();
void info();

int main() {
    system("chcp 437");
    system("CLS");
    wchar_t MapName[250];
    HANDLE mapping = NULL;
    LPVOID lpFileMap = NULL;
    int selected = -1;
    info();
    while (selected != 5) {
        cout << "\nType an option and press enter: ";
        selected = num_cin();
        switch (selected) {
            case 1: {
                cout << "Enter file mapping name (never, for example): ";
                (void)getchar();
                wcin.getline(MapName, 250, L'\n');

                mapping = OpenFileMapping(              // The OpenFileMapping function opens a named "projected file" object
                    FILE_MAP_READ | FILE_MAP_WRITE,     // access mode
                    FALSE,                              // inheritance flag
                    (LPCTSTR)MapName);                  // object name
                if (mapping == INVALID_HANDLE_VALUE or mapping == NULL) {
                    cout << "File open Error 0x" << GetLastError() << endl;
                    SetLastError(0);
                }
                else cout << "File open successfully" << endl;
                break;
            }
            case 2: {
                if (mapping == NULL) {
                    cout << "File was not opened\n";
                    break;
                }
                lpFileMap = MapViewOfFile(  // The MapViewOfFile function displays a representation of the projected file in the address space of the calling process.
                    mapping,                // desc. of the projected file object
                    FILE_MAP_ALL_ACCESS,    // access mode
                    0,                      // highest DWORD offset 
                    0,                      // lowest DWORD offset 
                    0);                     // number of bytes displayed
                if (!lpFileMap) cout << "Error in file projection\n";
                else cout << "File projection found!" << endl;
                break;
            }
            case 3: {
                if (lpFileMap == NULL) {
                    cout << "Fail to read data\n";
                    break;
                }
                cout << "Address: " << lpFileMap << "\nProjection's data: \n";
                cout << (char*)lpFileMap << endl;
                break;
            }
            case 4: {
                if (!UnmapViewOfFile(lpFileMap)) cout << "Unable to ummap view of file\n";  // The UnmapViewOfFile function cancels the display of the file view from the address space of the calling process.
                else cout << "View of file was unmap successfully\n";
                if (!CloseHandle(mapping)) cout << "Unable to close map file\n";            // The CloseHandle function closes the handle of an open object.
                else cout << "Map file close successfully\n";
                break;
            }
            case 5: {
                cout << "\nProgram exited\n";
                return 0;
            }
            default: {
                cout << "Wrong option!";
                cout << endl;
                break;
            }
        }
        system("pause");
    }
}

int num_cin() {
    int choice = -1;
    char str[50];
    cin >> str;
    while (sscanf(str, "%d", &choice) != 1) {
        printf("Incorrect input! Try again use only numbers:");
        cin >> str;
    }
    fflush(stdout);
    return choice;
}

void info() {
    cout << "This is a console application by Artem Ivanov,\n";
    cout << "a student of group 0305, for laboratory work\n";
    cout << "on the study of Memory Managment using the Win32 API.\n\n";
    cout << "\n-----THIS IS THE READER APP-----\n";

    cout << " '1' - open a file mapping object\n"
        << " '2' - create mapping\n"
        << " '3' - show mapped data\n"
        << " '4' - unmap a created map\n"
        << " '5' - exit\n"
        << "Please type only a number of an option without <' '>.\n" << endl;
}