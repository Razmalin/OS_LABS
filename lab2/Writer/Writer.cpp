#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <windows.h>
#include <string>

using namespace std;

int num_cin();
void info();

int main() {
    system("chcp 437");
    system("CLS");
    HANDLE file = NULL, mapping = NULL;
    LPVOID lpFileMap = NULL;
    std::string data;
    wchar_t FileName[250], MapName[250];
    int selected = -1;
    info();
    while (selected != 6) {
        cout << "\nType an option and press enter: ";
        selected = num_cin();
        switch (selected) {
            // create new file
            case 1: {
                cout << "Enter file name (moscow.txt, for example): ";
                (void)getchar();
                wcin.getline(FileName, MAX_PATH, L'\n');

                file = CreateFile(
                    FileName,                       // file name
                    GENERIC_WRITE | GENERIC_READ,   // access mode
                    0,                              // shared access
                    NULL,                           // SD (protection)
                    CREATE_ALWAYS,                  // what to do
                    FILE_ATTRIBUTE_NORMAL,          // file attributes
                    NULL);                          // desc.pattern of file

                if (file == INVALID_HANDLE_VALUE) {
                    cout << "Error 0x" << GetLastError() << endl;
                    SetLastError(0);

                }
                else cout << "File created successfully" << endl;
                break;
            }
            // make a mapping object
            case 2: {
                if (file == NULL) {
                    cout << "File was not created\n";
                    break;
                }
                cout << "Enter file mapping name (never, for example): ";
                (void)getchar();
                wcin.getline(MapName, MAX_PATH, L'\n');
                mapping = CreateFileMapping(file, NULL, PAGE_READWRITE, 0, 128, (LPCTSTR)&MapName[0]);

                if (mapping == NULL) {
                    cout << "Error 0x" << GetLastError() << endl;
                    SetLastError(0);
                }
                else
                    cout << "Mapping object created succsessfully\n";
                break;
            }
            // make a map
            case 3: {
                lpFileMap = MapViewOfFile(      // The MapViewOfFile function displays a representation of the projected file in the address space of the calling process
                    mapping,                    // desc. of the projected file object
                    FILE_MAP_ALL_ACCESS,        // access mode
                    0,                          // highest DWORD offset 
                    0,                          // lowest DWORD offset 
                    0);                         // number of bytes displayed
                if (lpFileMap != NULL)
                {
                    cout << "Map created successfully\n";
                    cout << "Projection's address: " << lpFileMap << endl;
                }
                else {
                    cout << "Mapping object wasnt created\nError 0x" << GetLastError() << endl;
                    SetLastError(0);
                }
                break;
            }
            // write data
            case 4: { 
                if (lpFileMap == NULL) {
                    cout << "Nowhere to write\n";
                    break;
                }
                cout << "Enter data for input (sleep, for example): ";
                (void)getchar();
                getline(cin, data);

                CopyMemory(lpFileMap, data.c_str(), data.length() * sizeof(char)); // CopyMemory copies a block of memory from one location to another using pointers

                cout << "Data successfully entered\nDon't close writer-file and open reader-file\n";
                break;
            }
            // unmap
            case 5: {
                if (lpFileMap != NULL && file != NULL) {
                    UnmapViewOfFile(lpFileMap);     // The UnmapViewOfFile function cancels the display of the file view from the address space of the calling process.
                    CloseHandle(file);              // The CloseHandle function closes the handle of an open object.
                }
                else {
                    cout << "File or mapping was not created\n";
                }
                break;
            }
            case 6: {
                cout << "\nProgram exited\n";
                return 0;
            }
            default: {
                cout << "\nWrong option!\n";
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
    cout << "\n-----THIS IS THE WRITTER APP-----\n";

    cout << " '1' - create new file\n"
        << " '2' - create a mapping object for created file\n"
        << " '3' - create mapping for created mapping object\n"
        << " '4' - write data to map\n"
        << " '5' - unmap a created map\n"
        << " '6' - exit\n"
        << "Please type only a number of an option without <' '>." << endl;
}