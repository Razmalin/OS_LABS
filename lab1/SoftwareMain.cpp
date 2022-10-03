#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include "DriversHeader.h"
#include "DirectoryHeader.h"
#include "FileHeader.h"
#include "FileAttributesHeader.h"
#include "OverlappingHeader.h"


using namespace std;

void info() {
    cout << " '1' - display list of disks\n"
        << " '2' - display info about disks\n"
        << " '3' - create or delete directory\n"
        << " '4' - create files in new directories\n"
        << " '5' - copying and moving files between directories\n"
        << " '6' - analyzing and changing file attributes\n"
        << " '7' - start copying file asyncronously (1.2)\n"
        << " '8' - exit\n\n"
        << "Please type only a number of an option without <' '>.\n" << endl;
}

int main() {

    system("chcp 437");
    int selected = -1;
    while (selected != 8) {
        system("cls");
        cout << "This is a console application by Artem Ivanov,\n";
        cout << "a student of group 0305, for laboratory work\n";
        cout << "on the study of file systemmanagement using the Win32 API.\n\n";
        info();
        cout << "Type an option and press enter: ";
        selected = num_cin();

        if (selected >= 1 && selected <= 8) {
            if (selected != 8)
                if (selected == 1)
                    printDrivesList();
                if (selected == 2)
                    AboutDiskHelp();
                if (selected == 3)
                    removeORdelete();
                if (selected == 4)
                    createFile();
                if (selected == 5)
                    copyAndReplaceFiles();
                if (selected == 6)
                    FileAttributes();
                if (selected == 7)
                    Overlapping();
        }
        else cout << "\nWrong option!\n";
        
        cout << endl;
        system("pause");
    }

    cout << "\nProgram exited.\n";
    return 0;
}